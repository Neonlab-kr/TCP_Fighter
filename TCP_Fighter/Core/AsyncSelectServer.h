#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <windows.h>
#include <memory>
#include <cstdint>

#include "Config.h"
#include "Session.h"

class CAsyncSelectServer
{
public:
    CAsyncSelectServer();
    virtual ~CAsyncSelectServer();

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

    template <typename TPacket>
    inline bool SendUnicast(CSession* session, const TPacket& packet)
    {
        return SendUnicast(session, reinterpret_cast<const char*>(&packet), static_cast<int>(sizeof(TPacket)));
    }

    template <typename TPacket>
    inline void SendBroadcast(CSession* exceptSession, const TPacket& packet)
    {
        SendBroadcast(exceptSession, reinterpret_cast<const char*>(&packet), static_cast<int>(sizeof(TPacket)));
    }

private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    bool CreateMessageWindow();
    void DestroyMessageWindow();
    void OnNetworkMessage(WPARAM wParam, LPARAM lParam);
    void AcceptProc();
    void RecvProc(CSession* session);
    void SendProc(CSession* session);
    CSession* FindSession(SOCKET socket);
    CSession* FindEmptySessionSlot();
    void RemovePendingSessions();

private:
    ServerConfig m_Config;
    SOCKET m_ListenSocket;
    HWND m_hWnd;
    std::uint64_t m_NextSessionId;
    bool m_Initialized;
    std::unique_ptr<CSession[]> m_Sessions;
    int m_MaxSession;
    int m_ActiveSessionCount;
};
