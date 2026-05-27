#include "GameServer.h"
#include "../Core/Session.h"

bool CGameServer::OnMoveStart(CSession* session, std::uint8_t direction, std::int16_t x, std::int16_t y)
{
    if (!ValidateDirection(direction))
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

bool CGameServer::OnMoveStop(CSession* session, std::uint8_t direction, std::int16_t x, std::int16_t y)
{
    if (!ValidateDirection(direction))
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

bool CGameServer::OnAttack1(CSession* session, std::uint8_t direction, std::int16_t x, std::int16_t y)
{
    if (!ValidateDirection(direction))
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

    RequestAttack(*gameSession, dfPACKET_C2S_ATTACK1, direction, x, y, session);
    return true;
}

bool CGameServer::OnAttack2(CSession* session, std::uint8_t direction, std::int16_t x, std::int16_t y)
{
    if (!ValidateDirection(direction))
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

    RequestAttack(*gameSession, dfPACKET_C2S_ATTACK2, direction, x, y, session);
    return true;
}

bool CGameServer::OnAttack3(CSession* session, std::uint8_t direction, std::int16_t x, std::int16_t y)
{
    if (!ValidateDirection(direction))
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

    RequestAttack(*gameSession, dfPACKET_C2S_ATTACK3, direction, x, y, session);
    return true;
}

bool CGameServer::OnSync(CSession* session, std::int16_t x, std::int16_t y)
{
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
