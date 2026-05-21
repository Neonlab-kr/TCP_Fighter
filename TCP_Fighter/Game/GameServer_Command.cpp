#include "GameServer.h"
#include "PacketMaker.h"
#include "GameMoveLogic.h"
#include "GameAttackLogic.h"
#include "../Core/Logger.h"
#include "../Core/Session.h"

void CGameServer::FixedUpdateGame()
{
    for (int i = 0; i < m_MaxGameSession; ++i)
    {
        GameSession& gameSession = m_GameSessions[i];

        if (!gameSession.Active)
            continue;

        CSession* session = gameSession.NetSession;
        if (session == nullptr || session->DisconnectPending)
            continue;

        if (gameSession.Moving)
            UpdateMove(gameSession);
    }
}

void CGameServer::StartMove(GameSession& gameSession, std::uint8_t direction, short clientX, short clientY, CSession* requestSession)
{
    if (IsAttackLocked(gameSession, m_CurrentTimeMs))
    {
        SendSync(gameSession.PlayerId, gameSession.X, gameSession.Y, requestSession);
        return;
    }

    if (!IsClientPositionInErrorRange(gameSession, clientX, clientY))
    {
        SendSync(gameSession.PlayerId, gameSession.X, gameSession.Y, requestSession);
        return;
    }

    gameSession.Direction = direction;
    gameSession.Moving = true;

    CPacket sendPacket(dfPACKET_MAX_SIZE);
    MakePacket_SC_MOVE_START(sendPacket, gameSession.PlayerId, direction, gameSession.X, gameSession.Y);
    SendBroadcast(requestSession, sendPacket);
}

void CGameServer::StopMoveRequest(GameSession& gameSession, std::uint8_t direction, short clientX, short clientY, CSession* requestSession)
{
    if (!IsClientPositionInErrorRange(gameSession, clientX, clientY))
    {
        SendSync(gameSession.PlayerId, gameSession.X, gameSession.Y, requestSession);
        return;
    }

    gameSession.Direction = direction;
    gameSession.Moving = false;

    CPacket sendPacket(dfPACKET_MAX_SIZE);
    MakePacket_SC_MOVE_STOP(sendPacket, gameSession.PlayerId, direction, gameSession.X, gameSession.Y);
    SendBroadcast(requestSession, sendPacket);
}

void CGameServer::UpdateMove(GameSession& gameSession)
{
    short nextX = gameSession.X;
    short nextY = gameSession.Y;

    if (!TryCalcNextPosition(gameSession.X, gameSession.Y, gameSession.Direction, m_GameData, nextX, nextY))
    {
        StopMove(gameSession, true);
        return;
    }

    gameSession.X = nextX;
    gameSession.Y = nextY;
}

void CGameServer::StopMove(GameSession& gameSession, bool sendPacket)
{
    if (!gameSession.Moving && !sendPacket)
        return;

    gameSession.Moving = false;

    if (sendPacket)
        SendMoveStop(gameSession.PlayerId, gameSession.Direction, gameSession.X, gameSession.Y);
}

void CGameServer::RequestAttack(GameSession& attacker, std::uint8_t attackType, std::uint8_t direction, short clientX, short clientY, CSession* requestSession)
{
    const AttackGameData* attackData = GetAttackData(attackType);

    if (attackData == nullptr)
    {
        Disconnect(requestSession);
        return;
    }

    if (IsAttackLocked(attacker, m_CurrentTimeMs))
    {
        SendSync(attacker.PlayerId, attacker.X, attacker.Y, requestSession);
        return;
    }

    if (!IsClientPositionInErrorRange(attacker, clientX, clientY))
    {
        SendSync(attacker.PlayerId, attacker.X, attacker.Y, requestSession);
        return;
    }

    attacker.Direction = direction;
    attacker.Moving = false;

    const std::uint32_t lockMs = attackData->CooldownMs > m_GameData.Attack.InputAdvanceMs
        ? attackData->CooldownMs - m_GameData.Attack.InputAdvanceMs
        : 1;

    attacker.AttackLockUntilTimeMs = m_CurrentTimeMs + lockMs;

    SendAttackPacket(attackType, attacker.PlayerId, direction, attacker.X, attacker.Y, requestSession);
    ProcessAttack(attacker, attackType, *attackData);
}

void CGameServer::ProcessAttack(GameSession& attacker, std::uint8_t attackType, const AttackGameData& attackData)
{
    if (attackData.RangeX <= 0 || attackData.RangeY <= 0 || attackData.Damage == 0)
        return;

    int deadCount = 0;

    for (int i = 0; i < m_MaxGameSession; ++i)
    {
        GameSession& target = m_GameSessions[i];

        if (!target.Active)
            continue;

        CSession* targetSession = target.NetSession;
        if (targetSession == nullptr || targetSession->DisconnectPending)
            continue;

        if (target.PlayerId == attacker.PlayerId || target.HP == 0)
            continue;

        if (!IsAttackHitByRange(attacker.X, attacker.Y, attacker.Direction, target.X, target.Y, attackData.RangeX, attackData.RangeY))
            continue;

        if (target.HP <= attackData.Damage)
            target.HP = 0;
        else
            target.HP = static_cast<std::uint8_t>(target.HP - attackData.Damage);

        SendDamage(attacker.PlayerId, target.PlayerId, target.HP);

        if (target.HP == 0 && deadCount < m_MaxGameSession)
            m_DeadSessionBuffer[deadCount++] = targetSession;
    }

    for (int i = 0; i < deadCount; ++i)
        ProcessDeadPlayer(m_DeadSessionBuffer[i]);
}

void CGameServer::ProcessDeadPlayer(CSession* session)
{
    if (session == nullptr)
        return;

    GameSession* deadPlayer = FindGameSession(session);

    if (deadPlayer == nullptr || !deadPlayer->Active)
        return;

    CLogger::Info("Player dead. session=%llu, player=%d", session->GetSessionId(), deadPlayer->PlayerId);
    Disconnect(session);
}
