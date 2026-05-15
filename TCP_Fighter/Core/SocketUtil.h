#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mstcpip.h>

class CSocketUtil
{
public:
    static inline bool SetNonBlocking(SOCKET socket)
    {
        u_long on = 1;
        return ioctlsocket(socket, FIONBIO, &on) != SOCKET_ERROR;
    }

    static inline bool SetTcpNoDelay(SOCKET socket, bool enable)
    {
        BOOL option = enable ? TRUE : FALSE;
        return setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&option), sizeof(option)) != SOCKET_ERROR;
    }

    static inline void CloseSocket(SOCKET& socket)
    {
        if (socket != INVALID_SOCKET)
        {
            closesocket(socket);
            socket = INVALID_SOCKET;
        }
    }
};
