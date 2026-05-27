#include "GameServer.h"

bool CGameServer::SendRPCPacket(CSession* session, const CPacket& packet)
{
    return SendUnicast(session, packet);
}

void CGameServer::BroadcastCreateOtherCharacter(int id, std::uint8_t direction, short x, short y, std::uint8_t hp, CSession* exceptSession)
{
    for (int i = 0; i < m_MaxGameSession; ++i)
    {
        const GameSession& target = m_GameSessions[i];

        if (!target.Active)
            continue;

        CSession* targetSession = target.NetSession;
        if (targetSession == nullptr || targetSession->DisconnectPending)
            continue;

        if (targetSession == exceptSession)
            continue;

        m_S2CProxy.CreateOtherCharacter(targetSession, id, direction, x, y, hp);
    }
}

void CGameServer::BroadcastDeleteCharacter(int id, CSession* exceptSession)
{
    for (int i = 0; i < m_MaxGameSession; ++i)
    {
        const GameSession& target = m_GameSessions[i];

        if (!target.Active)
            continue;

        CSession* targetSession = target.NetSession;
        if (targetSession == nullptr || targetSession->DisconnectPending)
            continue;

        if (targetSession == exceptSession)
            continue;

        m_S2CProxy.DeleteCharacter(targetSession, id);
    }
}

void CGameServer::SendMoveStart(int id, std::uint8_t direction, short x, short y, CSession* exceptSession)
{
    for (int i = 0; i < m_MaxGameSession; ++i)
    {
        const GameSession& target = m_GameSessions[i];

        if (!target.Active)
            continue;

        CSession* targetSession = target.NetSession;
        if (targetSession == nullptr || targetSession->DisconnectPending)
            continue;

        if (targetSession == exceptSession)
            continue;

        m_S2CProxy.MoveStart(targetSession, id, direction, x, y);
    }
}

void CGameServer::SendMoveStop(int id, std::uint8_t direction, short x, short y)
{
    for (int i = 0; i < m_MaxGameSession; ++i)
    {
        const GameSession& target = m_GameSessions[i];

        if (!target.Active)
            continue;

        CSession* targetSession = target.NetSession;
        if (targetSession == nullptr || targetSession->DisconnectPending)
            continue;

        m_S2CProxy.MoveStop(targetSession, id, direction, x, y);
    }
}

void CGameServer::SendSync(int id, short x, short y, CSession* targetSession)
{
    if (targetSession == nullptr)
        return;

    m_S2CProxy.Sync(targetSession, id, x, y);
}

void CGameServer::SendAttackPacket(PacketType attackType, int id, std::uint8_t direction, short x, short y, CSession* exceptSession)
{
    for (int i = 0; i < m_MaxGameSession; ++i)
    {
        const GameSession& target = m_GameSessions[i];

        if (!target.Active)
            continue;

        CSession* targetSession = target.NetSession;
        if (targetSession == nullptr || targetSession->DisconnectPending)
            continue;

        if (targetSession == exceptSession)
            continue;

        switch (attackType)
        {
        case dfPACKET_C2S_ATTACK1:
            m_S2CProxy.Attack1(targetSession, id, direction, x, y);
            break;

        case dfPACKET_C2S_ATTACK2:
            m_S2CProxy.Attack2(targetSession, id, direction, x, y);
            break;

        case dfPACKET_C2S_ATTACK3:
            m_S2CProxy.Attack3(targetSession, id, direction, x, y);
            break;

        default:
            return;
        }
    }
}

void CGameServer::SendDamage(int attackId, int damageId, std::uint8_t damageHp)
{
    for (int i = 0; i < m_MaxGameSession; ++i)
    {
        const GameSession& target = m_GameSessions[i];

        if (!target.Active)
            continue;

        CSession* targetSession = target.NetSession;
        if (targetSession == nullptr || targetSession->DisconnectPending)
            continue;

        m_S2CProxy.Damage(targetSession, attackId, damageId, damageHp);
    }
}
