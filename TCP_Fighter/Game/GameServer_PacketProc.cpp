#include "GameServer.h"
#include "PacketMaker.h"
#include "../Core/Session.h"

bool CGameServer::PacketProc_MoveStart(CSession* session, CPacket& packet)
{
    if (!ValidatePayloadSize(packet, dfPACKET_PAYLOAD_SIZE_CS_MOVE_START))
    {
        Disconnect(session);
        return false;
    }

    std::uint8_t direction = 0;
    short x = 0;
    short y = 0;
    if (!ReadPayload_CS_MOVE_START(packet, direction, x, y) || !ValidateDirection(direction))
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

    StartMove(*gameSession, direction, x, y, session);
    return true;
}

bool CGameServer::PacketProc_MoveStop(CSession* session, CPacket& packet)
{
    if (!ValidatePayloadSize(packet, dfPACKET_PAYLOAD_SIZE_CS_MOVE_STOP))
    {
        Disconnect(session);
        return false;
    }

    std::uint8_t direction = 0;
    short x = 0;
    short y = 0;
    if (!ReadPayload_CS_MOVE_STOP(packet, direction, x, y) || !ValidateDirection(direction))
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

    StopMoveRequest(*gameSession, direction, x, y, session);
    return true;
}

bool CGameServer::PacketProc_Attack1(CSession* session, CPacket& packet)
{
    if (!ValidatePayloadSize(packet, dfPACKET_PAYLOAD_SIZE_CS_ATTACK1))
    {
        Disconnect(session);
        return false;
    }

    std::uint8_t direction = 0;
    short x = 0;
    short y = 0;
    if (!ReadPayload_CS_ATTACK1(packet, direction, x, y) || !ValidateDirection(direction))
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

    RequestAttack(*gameSession, dfPACKET_CS_ATTACK1, direction, x, y, session);
    return true;
}

bool CGameServer::PacketProc_Attack2(CSession* session, CPacket& packet)
{
    if (!ValidatePayloadSize(packet, dfPACKET_PAYLOAD_SIZE_CS_ATTACK2))
    {
        Disconnect(session);
        return false;
    }

    std::uint8_t direction = 0;
    short x = 0;
    short y = 0;
    if (!ReadPayload_CS_ATTACK2(packet, direction, x, y) || !ValidateDirection(direction))
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

    RequestAttack(*gameSession, dfPACKET_CS_ATTACK2, direction, x, y, session);
    return true;
}

bool CGameServer::PacketProc_Attack3(CSession* session, CPacket& packet)
{
    if (!ValidatePayloadSize(packet, dfPACKET_PAYLOAD_SIZE_CS_ATTACK3))
    {
        Disconnect(session);
        return false;
    }

    std::uint8_t direction = 0;
    short x = 0;
    short y = 0;
    if (!ReadPayload_CS_ATTACK3(packet, direction, x, y) || !ValidateDirection(direction))
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

    RequestAttack(*gameSession, dfPACKET_CS_ATTACK3, direction, x, y, session);
    return true;
}

bool CGameServer::PacketProc_Sync(CSession* session, CPacket& packet)
{
    if (!ValidatePayloadSize(packet, dfPACKET_PAYLOAD_SIZE_CS_SYNC))
    {
        Disconnect(session);
        return false;
    }

    short x = 0;
    short y = 0;
    if (!ReadPayload_CS_SYNC(packet, x, y))
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

    if (!IsClientPositionInErrorRange(*gameSession, x, y))
        SendSync(gameSession->PlayerId, gameSession->X, gameSession->Y, session);

    return true;
}
