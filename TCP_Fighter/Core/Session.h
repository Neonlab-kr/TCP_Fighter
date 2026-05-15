#pragma once

#include <winsock2.h>
#include <cstdint>
#include "RingBuffer.h"

class CSession
{
public:
    CSession();
    ~CSession();

    CSession(const CSession&) = delete;
    CSession& operator=(const CSession&) = delete;

    void ReserveBuffers(int recvBufferSize, int sendBufferSize);
    void Initialize(std::uint64_t sessionId, int slotIndex, SOCKET socket, const sockaddr_in& addr);
    void Close();

    inline SOCKET GetSocket() const { return m_Socket; }
    inline std::uint64_t GetSessionId() const { return m_SessionId; }
    inline unsigned long GetIP() const { return m_Addr.sin_addr.s_addr; }
    inline unsigned short GetPort() const { return ntohs(m_Addr.sin_port); }
    inline int GetSlotIndex() const { return m_SlotIndex; }
    inline bool IsActive() const { return Active; }

public:
    RecvRingBuffer RecvBuffer;
    SendRingBuffer SendBuffer;
    void* UserData;
    bool Active;
    bool DisconnectPending;

private:
    SOCKET m_Socket;
    sockaddr_in m_Addr;
    std::uint64_t m_SessionId;
    int m_SlotIndex;
};
