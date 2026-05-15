#include "Session.h"
#include "SocketUtil.h"

CSession::CSession()
{
    m_Socket = INVALID_SOCKET;
    m_SessionId = 0;
    m_SlotIndex = -1;
    ZeroMemory(&m_Addr, sizeof(m_Addr));
    UserData = nullptr;
    Active = false;
    DisconnectPending = false;
}

CSession::~CSession()
{
    Close();
}

void CSession::ReserveBuffers(int recvBufferSize, int sendBufferSize)
{
    if (RecvBuffer.GetBufferSize() != recvBufferSize)
        RecvBuffer.Resize(recvBufferSize);

    if (SendBuffer.GetBufferSize() != sendBufferSize)
        SendBuffer.Resize(sendBufferSize);

    RecvBuffer.ClearBuffer();
    SendBuffer.ClearBuffer();
}

void CSession::Initialize(std::uint64_t sessionId, int slotIndex, SOCKET socket, const sockaddr_in& addr)
{
    m_SessionId = sessionId;
    m_SlotIndex = slotIndex;
    m_Socket = socket;
    m_Addr = addr;
    UserData = nullptr;
    Active = true;
    DisconnectPending = false;
    RecvBuffer.ClearBuffer();
    SendBuffer.ClearBuffer();
}

void CSession::Close()
{
    CSocketUtil::CloseSocket(m_Socket);
    RecvBuffer.ClearBuffer();
    SendBuffer.ClearBuffer();
    UserData = nullptr;
    Active = false;
    DisconnectPending = false;
}
