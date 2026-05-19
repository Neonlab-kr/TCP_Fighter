#include "GameServer.h"
#include "PacketDefine.h"
#include "../Core/Logger.h"
#include "../Core/Session.h"

void CGameServer::OnRecv(CSession* session)
{
    if (session == nullptr)
        return;

    while (true)
    {
        if (session->RecvBuffer.GetUseSize() < static_cast<int>(sizeof(st_PACKET_HEADER)))
            return;

        st_PACKET_HEADER header{};
        const int peekRet = session->RecvBuffer.Peek(reinterpret_cast<char*>(&header), sizeof(header));

        if (peekRet != sizeof(header))
            return;

        if (header.byCode != dfPACKET_CODE)
        {
            CLogger::Error("Invalid packet code. session=%llu, code=0x%02x", session->GetSessionId(), header.byCode);
            Disconnect(session);
            return;
        }

        const int packetSize = static_cast<int>(sizeof(st_PACKET_HEADER)) + header.bySize;

        if (packetSize <= static_cast<int>(sizeof(st_PACKET_HEADER)) || packetSize > dfPACKET_MAX_SIZE)
        {
            CLogger::Error("Invalid packet size. session=%llu, size=%d", session->GetSessionId(), packetSize);
            Disconnect(session);
            return;
        }

        if (session->RecvBuffer.GetUseSize() < packetSize)
            return;

        char packet[dfPACKET_MAX_SIZE]{};
        const int dequeueRet = session->RecvBuffer.Dequeue(packet, packetSize);

        if (dequeueRet != packetSize)
            return;

        ProcessPacket(session, packet, packetSize);

        if (session->DisconnectPending)
            return;
    }
}

void CGameServer::ProcessPacket(CSession* session, const char* packet, int packetSize)
{
    if (session == nullptr || packet == nullptr || packetSize <= 0)
        return;

    GameSession* gameSession = FindGameSession(session);

    if (gameSession == nullptr || !gameSession->Active)
    {
        Disconnect(session);
        return;
    }

    const st_PACKET_HEADER* header = reinterpret_cast<const st_PACKET_HEADER*>(packet);

    switch (header->byType)
    {
    case dfPACKET_CS_MOVE_START:
        PacketProc_MoveStart(session, packet, packetSize);
        break;

    case dfPACKET_CS_MOVE_STOP:
        PacketProc_MoveStop(session, packet, packetSize);
        break;

    case dfPACKET_CS_ATTACK1:
        PacketProc_Attack1(session, packet, packetSize);
        break;

    case dfPACKET_CS_ATTACK2:
        PacketProc_Attack2(session, packet, packetSize);
        break;

    case dfPACKET_CS_ATTACK3:
        PacketProc_Attack3(session, packet, packetSize);
        break;

    case dfPACKET_CS_SYNC:
        PacketProc_Sync(session, packet, packetSize);
        break;

    default:
        CLogger::Error("Unknown packet type. session=%llu, type=%u", session->GetSessionId(), header->byType);
        Disconnect(session);
        break;
    }
}
