#include "GameServer.h"
#include "PacketDefine.h"
#include "../Core/Session.h"

bool CGameServer::PacketProc_MoveStart(CSession* session, const char* packet, int packetSize)
{
    if (!ValidatePacketSize(packet, packetSize, sizeof(st_PACKET_CS_MOVE_START)))
    {
        Disconnect(session);
        return false;
    }

    const st_PACKET_CS_MOVE_START* recvPacket = reinterpret_cast<const st_PACKET_CS_MOVE_START*>(packet);

    if (!ValidateDirection(recvPacket->direction))
    {
        Disconnect(session);
        return false;
    }

    GameSession* gameSession = FindGameSession(session);
    if (gameSession == nullptr || !gameSession->Active)
    {
        Disconnect(session);
        return false;
    }

    StartMove(*gameSession, recvPacket->direction, recvPacket->x, recvPacket->y, session);
    return true;
}

bool CGameServer::PacketProc_MoveStop(CSession* session, const char* packet, int packetSize)
{
    if (!ValidatePacketSize(packet, packetSize, sizeof(st_PACKET_CS_MOVE_STOP)))
    {
        Disconnect(session);
        return false;
    }

    const st_PACKET_CS_MOVE_STOP* recvPacket = reinterpret_cast<const st_PACKET_CS_MOVE_STOP*>(packet);

    if (!ValidateDirection(recvPacket->direction))
    {
        Disconnect(session);
        return false;
    }

    GameSession* gameSession = FindGameSession(session);
    if (gameSession == nullptr || !gameSession->Active)
    {
        Disconnect(session);
        return false;
    }

    StopMoveRequest(*gameSession, recvPacket->direction, recvPacket->x, recvPacket->y, session);
    return true;
}

bool CGameServer::PacketProc_Attack1(CSession* session, const char* packet, int packetSize)
{
    if (!ValidatePacketSize(packet, packetSize, sizeof(st_PACKET_CS_ATTACK1)))
    {
        Disconnect(session);
        return false;
    }

    const st_PACKET_CS_ATTACK1* recvPacket = reinterpret_cast<const st_PACKET_CS_ATTACK1*>(packet);

    if (!ValidateDirection(recvPacket->direction))
    {
        Disconnect(session);
        return false;
    }

    GameSession* gameSession = FindGameSession(session);
    if (gameSession == nullptr || !gameSession->Active)
    {
        Disconnect(session);
        return false;
    }

    RequestAttack(*gameSession, dfPACKET_CS_ATTACK1, recvPacket->direction, recvPacket->x, recvPacket->y, session);
    return true;
}

bool CGameServer::PacketProc_Attack2(CSession* session, const char* packet, int packetSize)
{
    if (!ValidatePacketSize(packet, packetSize, sizeof(st_PACKET_CS_ATTACK2)))
    {
        Disconnect(session);
        return false;
    }

    const st_PACKET_CS_ATTACK2* recvPacket = reinterpret_cast<const st_PACKET_CS_ATTACK2*>(packet);

    if (!ValidateDirection(recvPacket->direction))
    {
        Disconnect(session);
        return false;
    }

    GameSession* gameSession = FindGameSession(session);
    if (gameSession == nullptr || !gameSession->Active)
    {
        Disconnect(session);
        return false;
    }

    RequestAttack(*gameSession, dfPACKET_CS_ATTACK2, recvPacket->direction, recvPacket->x, recvPacket->y, session);
    return true;
}

bool CGameServer::PacketProc_Attack3(CSession* session, const char* packet, int packetSize)
{
    if (!ValidatePacketSize(packet, packetSize, sizeof(st_PACKET_CS_ATTACK3)))
    {
        Disconnect(session);
        return false;
    }

    const st_PACKET_CS_ATTACK3* recvPacket = reinterpret_cast<const st_PACKET_CS_ATTACK3*>(packet);

    if (!ValidateDirection(recvPacket->direction))
    {
        Disconnect(session);
        return false;
    }

    GameSession* gameSession = FindGameSession(session);
    if (gameSession == nullptr || !gameSession->Active)
    {
        Disconnect(session);
        return false;
    }

    RequestAttack(*gameSession, dfPACKET_CS_ATTACK3, recvPacket->direction, recvPacket->x, recvPacket->y, session);
    return true;
}

bool CGameServer::PacketProc_Sync(CSession* session, const char* packet, int packetSize)
{
    if (!ValidatePacketSize(packet, packetSize, sizeof(st_PACKET_CS_SYNC)))
    {
        Disconnect(session);
        return false;
    }

    const st_PACKET_CS_SYNC* recvPacket = reinterpret_cast<const st_PACKET_CS_SYNC*>(packet);

    GameSession* gameSession = FindGameSession(session);
    if (gameSession == nullptr || !gameSession->Active)
    {
        Disconnect(session);
        return false;
    }

    if (!IsClientPositionInErrorRange(*gameSession, recvPacket->x, recvPacket->y))
        SendSync(gameSession->PlayerId, gameSession->X, gameSession->Y, session);

    return true;
}
