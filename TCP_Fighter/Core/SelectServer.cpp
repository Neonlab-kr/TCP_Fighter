#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "SelectServer.h"
#include "SocketUtil.h"
#include "Logger.h"

#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

CSelectServer::CSelectServer()
{
    m_ListenSocket = INVALID_SOCKET;
    m_NextSessionId = 1;
    m_Initialized = false;
    m_MaxSession = 0;
    m_ActiveSessionCount = 0;
}

CSelectServer::~CSelectServer()
{
    Release();
}

bool CSelectServer::Init(const ServerConfig& config)
{
    m_Config = config;
    m_MaxSession = m_Config.MaxSession;

    if (m_MaxSession <= 0)
        m_MaxSession = SELECT_MAX_CLIENT_SESSION;

    if (m_MaxSession > SELECT_MAX_CLIENT_SESSION)
        m_MaxSession = SELECT_MAX_CLIENT_SESSION;

    m_ActiveSessionCount = 0;
    m_Sessions.reset(new CSession[m_MaxSession]);

    for (int i = 0; i < m_MaxSession; ++i)
        m_Sessions[i].ReserveBuffers(m_Config.RecvBufferSize, m_Config.SendBufferSize);

    WSADATA wsaData{};
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        CLogger::Error("WSAStartup failed");
        return false;
    }

    m_Initialized = true;

    m_ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_ListenSocket == INVALID_SOCKET)
    {
        CLogger::Error("listen socket failed: %d", WSAGetLastError());
        Release();
        return false;
    }

    BOOL reuse = TRUE;
    setsockopt(m_ListenSocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&reuse), sizeof(reuse));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_Config.Port);

    if (m_Config.BindIP.empty() || m_Config.BindIP == "0.0.0.0")
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
    else
        addr.sin_addr.s_addr = inet_addr(m_Config.BindIP.c_str());

    if (bind(m_ListenSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR)
    {
        CLogger::Error("bind failed: %d", WSAGetLastError());
        Release();
        return false;
    }

    if (listen(m_ListenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        CLogger::Error("listen failed: %d", WSAGetLastError());
        Release();
        return false;
    }

    if (!CSocketUtil::SetNonBlocking(m_ListenSocket))
    {
        CLogger::Error("listen nonblocking failed: %d", WSAGetLastError());
        Release();
        return false;
    }

    CLogger::Info("Select server started. %s:%hu, ClientFPS=%d, MaxSession=%d", m_Config.BindIP.c_str(), m_Config.Port, m_Config.ClientFPS, m_MaxSession);
    return true;
}

void CSelectServer::UpdateNetwork()
{
    if (!m_Initialized || m_ListenSocket == INVALID_SOCKET)
        return;

    fd_set readSet;
    fd_set writeSet;
    bool hasWriteSocket = false;

    BuildFdSets(readSet, writeSet, hasWriteSocket);

    timeval timeout{};
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    const int ret = select(0, &readSet, hasWriteSocket ? &writeSet : nullptr, nullptr, &timeout);

    if (ret == SOCKET_ERROR)
    {
        OnError(WSAGetLastError());
        RemovePendingSessions();
        return;
    }

    if (ret > 0)
    {
        if (FD_ISSET(m_ListenSocket, &readSet))
            AcceptProc();

        for (int i = 0; i < m_MaxSession; ++i)
        {
            CSession& session = m_Sessions[i];

            if (!session.IsActive() || session.DisconnectPending)
                continue;

            if (FD_ISSET(session.GetSocket(), &readSet))
                RecvProc(&session);

            if (!session.IsActive() || session.DisconnectPending)
                continue;

            if (hasWriteSocket && FD_ISSET(session.GetSocket(), &writeSet))
                SendProc(&session);
        }
    }

    RemovePendingSessions();
}

void CSelectServer::Release()
{
    if (!m_Initialized && m_ListenSocket == INVALID_SOCKET && m_Sessions == nullptr)
        return;

    if (m_Sessions != nullptr)
    {
        for (int i = 0; i < m_MaxSession; ++i)
        {
            if (m_Sessions[i].IsActive())
                m_Sessions[i].Close();
        }

        m_Sessions.reset();
    }

    m_MaxSession = 0;
    m_ActiveSessionCount = 0;

    CSocketUtil::CloseSocket(m_ListenSocket);

    if (m_Initialized)
    {
        WSACleanup();
        m_Initialized = false;
    }
}

bool CSelectServer::SendUnicast(CSession* session, const char* buffer, int size)
{
    if (session == nullptr || buffer == nullptr || size <= 0)
        return false;

    if (!session->IsActive() || session->DisconnectPending)
        return false;

    const int written = session->SendBuffer.Enqueue(buffer, size);

    if (written != size)
    {
        Disconnect(session);
        return false;
    }

    return true;
}

void CSelectServer::SendBroadcast(CSession* exceptSession, const char* buffer, int size)
{
    if (buffer == nullptr || size <= 0 || m_Sessions == nullptr)
        return;

    for (int i = 0; i < m_MaxSession; ++i)
    {
        CSession& session = m_Sessions[i];

        if (!session.IsActive() || session.DisconnectPending)
            continue;

        if (&session == exceptSession)
            continue;

        SendUnicast(&session, buffer, size);
    }
}

void CSelectServer::Disconnect(CSession* session)
{
    if (session == nullptr)
        return;

    if (!session->IsActive() || session->DisconnectPending)
        return;

    session->DisconnectPending = true;
}

void CSelectServer::BuildFdSets(fd_set& readSet, fd_set& writeSet, bool& hasWriteSocket)
{
    FD_ZERO(&readSet);
    FD_ZERO(&writeSet);

    hasWriteSocket = false;

    FD_SET(m_ListenSocket, &readSet);

    for (int i = 0; i < m_MaxSession; ++i)
    {
        CSession& session = m_Sessions[i];

        if (!session.IsActive() || session.DisconnectPending)
            continue;

        FD_SET(session.GetSocket(), &readSet);

        if (session.SendBuffer.GetUseSize() > 0)
        {
            FD_SET(session.GetSocket(), &writeSet);
            hasWriteSocket = true;
        }
    }
}

void CSelectServer::AcceptProc()
{
    while (true)
    {
        sockaddr_in clientAddr{};
        int addrLen = sizeof(clientAddr);

        SOCKET clientSocket = accept(m_ListenSocket, reinterpret_cast<sockaddr*>(&clientAddr), &addrLen);

        if (clientSocket == INVALID_SOCKET)
        {
            const int err = WSAGetLastError();

            if (err != WSAEWOULDBLOCK)
                CLogger::Error("accept failed: %d", err);

            return;
        }

        if (m_ActiveSessionCount >= m_MaxSession)
        {
            CLogger::Error("Accept rejected. session full. active=%d, max=%d", m_ActiveSessionCount, m_MaxSession);
            closesocket(clientSocket);
            continue;
        }

        CSession* session = FindEmptySessionSlot();

        if (session == nullptr)
        {
            CLogger::Error("Accept rejected. empty session slot not found. active=%d, max=%d", m_ActiveSessionCount, m_MaxSession);
            closesocket(clientSocket);
            continue;
        }

        if (!CSocketUtil::SetNonBlocking(clientSocket))
        {
            closesocket(clientSocket);
            continue;
        }

        CSocketUtil::SetTcpNoDelay(clientSocket, m_Config.TcpNoDelay);

        const int slotIndex = static_cast<int>(session - m_Sessions.get());
        session->Initialize(m_NextSessionId++, slotIndex, clientSocket, clientAddr);
        ++m_ActiveSessionCount;

        OnAccept(session);
    }
}

void CSelectServer::RecvProc(CSession* session)
{
    if (session == nullptr || !session->IsActive() || session->DisconnectPending)
        return;

    while (true)
    {
        const int directSize = session->RecvBuffer.DirectEnqueueSize();

        if (directSize <= 0)
        {
            Disconnect(session);
            return;
        }

        const int ret = recv(session->GetSocket(), session->RecvBuffer.GetRearBufferPtr(), directSize, 0);

        if (ret == 0)
        {
            Disconnect(session);
            return;
        }

        if (ret == SOCKET_ERROR)
        {
            const int err = WSAGetLastError();

            if (err == WSAEWOULDBLOCK)
                break;

            Disconnect(session);
            return;
        }

        session->RecvBuffer.MoveRear(ret);

        if (ret < directSize)
            break;
    }

    OnRecv(session);
}

void CSelectServer::SendProc(CSession* session)
{
    if (session == nullptr || !session->IsActive() || session->DisconnectPending)
        return;

    while (session->SendBuffer.GetUseSize() > 0)
    {
        const int directSize = session->SendBuffer.DirectDequeueSize();

        if (directSize <= 0)
            return;

        const int ret = send(session->GetSocket(), session->SendBuffer.GetFrontBufferPtr(), directSize, 0);

        if (ret == SOCKET_ERROR)
        {
            const int err = WSAGetLastError();

            if (err == WSAEWOULDBLOCK)
                return;

            Disconnect(session);
            return;
        }

        if (ret == 0)
        {
            Disconnect(session);
            return;
        }

        session->SendBuffer.MoveFront(ret);

        if (ret < directSize)
            return;
    }
}

CSession* CSelectServer::FindEmptySessionSlot()
{
    if (m_Sessions == nullptr)
        return nullptr;

    for (int i = 0; i < m_MaxSession; ++i)
    {
        if (!m_Sessions[i].IsActive())
            return &m_Sessions[i];
    }

    return nullptr;
}

void CSelectServer::RemovePendingSessions()
{
    if (m_Sessions == nullptr)
        return;

    for (int i = 0; i < m_MaxSession; ++i)
    {
        CSession& session = m_Sessions[i];

        if (!session.IsActive() || !session.DisconnectPending)
            continue;

        OnRelease(&session);
        session.Close();

        if (m_ActiveSessionCount > 0)
            --m_ActiveSessionCount;
    }
}
