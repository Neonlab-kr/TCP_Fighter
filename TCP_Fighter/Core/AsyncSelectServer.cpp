#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "AsyncSelectServer.h"
#include "SocketUtil.h"
#include "Logger.h"

#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

static const UINT WM_ASYNC_NETWORK = WM_USER + 100;
static const wchar_t* ASYNC_SELECT_WINDOW_CLASS = L"ProcademyAsyncSelectServerWindow";

CAsyncSelectServer::CAsyncSelectServer()
{
    m_ListenSocket = INVALID_SOCKET;
    m_hWnd = nullptr;
    m_NextSessionId = 1;
    m_Initialized = false;
    m_MaxSession = 0;
    m_ActiveSessionCount = 0;
}

CAsyncSelectServer::~CAsyncSelectServer()
{
    Release();
}

bool CAsyncSelectServer::Init(const ServerConfig& config)
{
    m_Config = config;
    m_MaxSession = m_Config.MaxSession > 0 ? m_Config.MaxSession : 64;
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

    if (!CreateMessageWindow())
    {
        CLogger::Error("CreateMessageWindow failed");
        WSACleanup();
        m_Initialized = false;
        return false;
    }

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

    if (WSAAsyncSelect(m_ListenSocket, m_hWnd, WM_ASYNC_NETWORK, FD_ACCEPT | FD_CLOSE) == SOCKET_ERROR)
    {
        CLogger::Error("WSAAsyncSelect listen failed: %d", WSAGetLastError());
        Release();
        return false;
    }

    CLogger::Info("AsyncSelect server started. %s:%hu, ClientFPS=%d, MaxSession=%d", m_Config.BindIP.c_str(), m_Config.Port, m_Config.ClientFPS, m_MaxSession);
    return true;
}

void CAsyncSelectServer::UpdateNetwork()
{
    MSG msg{};

    while (PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    RemovePendingSessions();
}

void CAsyncSelectServer::Release()
{
    if (!m_Initialized && m_ListenSocket == INVALID_SOCKET && m_hWnd == nullptr && m_Sessions == nullptr)
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
    DestroyMessageWindow();

    if (m_Initialized)
    {
        WSACleanup();
        m_Initialized = false;
    }
}

bool CAsyncSelectServer::SendUnicast(CSession* session, const char* buffer, int size)
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

    SendProc(session);
    return session->IsActive() && !session->DisconnectPending;
}

void CAsyncSelectServer::SendBroadcast(CSession* exceptSession, const char* buffer, int size)
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

void CAsyncSelectServer::Disconnect(CSession* session)
{
    if (session == nullptr)
        return;

    if (!session->IsActive() || session->DisconnectPending)
        return;

    session->DisconnectPending = true;
    OnRelease(session);
}

LRESULT CALLBACK CAsyncSelectServer::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    CAsyncSelectServer* server = reinterpret_cast<CAsyncSelectServer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    if (msg == WM_NCCREATE)
    {
        CREATESTRUCT* createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
        server = reinterpret_cast<CAsyncSelectServer*>(createStruct->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(server));
        return TRUE;
    }

    if (msg == WM_ASYNC_NETWORK && server != nullptr)
    {
        server->OnNetworkMessage(wParam, lParam);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool CAsyncSelectServer::CreateMessageWindow()
{
    HINSTANCE hInstance = GetModuleHandle(nullptr);

    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = CAsyncSelectServer::WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = ASYNC_SELECT_WINDOW_CLASS;

    RegisterClassExW(&wc);

    m_hWnd = CreateWindowExW(0, ASYNC_SELECT_WINDOW_CLASS, L"", 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr, hInstance, this);
    return m_hWnd != nullptr;
}

void CAsyncSelectServer::DestroyMessageWindow()
{
    if (m_hWnd != nullptr)
    {
        DestroyWindow(m_hWnd);
        m_hWnd = nullptr;
    }
}

void CAsyncSelectServer::OnNetworkMessage(WPARAM wParam, LPARAM lParam)
{
    SOCKET socket = static_cast<SOCKET>(wParam);
    const int event = WSAGETSELECTEVENT(lParam);
    const int error = WSAGETSELECTERROR(lParam);

    if (error != 0)
    {
        if (socket == m_ListenSocket)
        {
            OnError(error);
            return;
        }

        Disconnect(FindSession(socket));
        return;
    }

    if (socket == m_ListenSocket)
    {
        if (event == FD_ACCEPT)
            AcceptProc();
        return;
    }

    CSession* session = FindSession(socket);
    if (session == nullptr)
        return;

    switch (event)
    {
    case FD_READ:
        RecvProc(session);
        break;

    case FD_WRITE:
        SendProc(session);
        break;

    case FD_CLOSE:
        Disconnect(session);
        break;

    default:
        break;
    }
}

void CAsyncSelectServer::AcceptProc()
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
            closesocket(clientSocket);
            continue;
        }

        CSession* session = FindEmptySessionSlot();
        if (session == nullptr)
        {
            closesocket(clientSocket);
            continue;
        }

        CSocketUtil::SetTcpNoDelay(clientSocket, m_Config.TcpNoDelay);

        if (WSAAsyncSelect(clientSocket, m_hWnd, WM_ASYNC_NETWORK, FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
        {
            closesocket(clientSocket);
            continue;
        }

        const int slotIndex = static_cast<int>(session - m_Sessions.get());
        session->Initialize(m_NextSessionId++, slotIndex, clientSocket, clientAddr);
        ++m_ActiveSessionCount;

        OnAccept(session);
    }
}

void CAsyncSelectServer::RecvProc(CSession* session)
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

void CAsyncSelectServer::SendProc(CSession* session)
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

CSession* CAsyncSelectServer::FindSession(SOCKET socket)
{
    if (m_Sessions == nullptr)
        return nullptr;

    for (int i = 0; i < m_MaxSession; ++i)
    {
        CSession& session = m_Sessions[i];
        if (session.IsActive() && session.GetSocket() == socket)
            return &session;
    }

    return nullptr;
}

CSession* CAsyncSelectServer::FindEmptySessionSlot()
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

void CAsyncSelectServer::RemovePendingSessions()
{
    if (m_Sessions == nullptr)
        return;

    for (int i = 0; i < m_MaxSession; ++i)
    {
        CSession& session = m_Sessions[i];

        if (!session.IsActive() || !session.DisconnectPending)
            continue;

        session.Close();
        --m_ActiveSessionCount;
    }
}
