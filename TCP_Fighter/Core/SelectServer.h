#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <memory>
#include <cstdint>

#include "Config.h"
#include "Session.h"
#include "SerializationBuffer.h"

class CSelectServer
{
public:
    CSelectServer();
    virtual ~CSelectServer();

    bool Init(const ServerConfig& config);
    void UpdateNetwork();
    void Release();

    void Disconnect(CSession* session);

    inline int GetSessionCount() const { return m_ActiveSessionCount; }
    inline int GetMaxSession() const { return m_MaxSession; }

protected:
    virtual void OnAccept(CSession* session) {}
    virtual void OnRelease(CSession* session) {}
    virtual void OnRecv(CSession* session) {}
    virtual void OnError(int errorCode) {}

    bool SendUnicast(CSession* session, const char* buffer, int size);
    void SendBroadcast(CSession* exceptSession, const char* buffer, int size);

    inline bool SendUnicast(CSession* session, const CPacket& packet)
    {
        return SendUnicast(session, packet.GetBufferPtr(), packet.GetDataSize());
    }

    inline void SendBroadcast(CSession* exceptSession, const CPacket& packet)
    {
        SendBroadcast(exceptSession, packet.GetBufferPtr(), packet.GetDataSize());
    }

private:
    void BuildFdSets(fd_set& readSet, fd_set& writeSet, bool& hasWriteSocket);
    void AcceptProc();
    void RecvProc(CSession* session);
    void SendProc(CSession* session);
    CSession* FindEmptySessionSlot();
    void RemovePendingSessions();

private:
    static constexpr int SELECT_SOCKET_LIMIT = FD_SETSIZE;
    static constexpr int SELECT_MAX_CLIENT_SESSION = FD_SETSIZE - 1;

    ServerConfig m_Config;
    SOCKET m_ListenSocket;
    std::uint64_t m_NextSessionId;
    bool m_Initialized;
    std::unique_ptr<CSession[]> m_Sessions;
    int m_MaxSession;
    int m_ActiveSessionCount;
};
