#include "GameServer.h"
#include "PacketMaker.h"

void CGameServer::SendMoveStop(int id, std::uint8_t direction, short x, short y)
{
    m_SendPacketBuffer.Clear();
    MakePacket_MoveStop(m_SendPacketBuffer, id, direction, x, y);
    SendBroadcast(nullptr, m_SendPacketBuffer);
}

void CGameServer::SendSync(int id, short x, short y, CSession* targetSession)
{
    if (targetSession == nullptr)
        return;

    m_SendPacketBuffer.Clear();
    MakePacket_Sync(m_SendPacketBuffer, id, x, y);
    SendUnicast(targetSession, m_SendPacketBuffer);
}

void CGameServer::SendAttackPacket(std::uint8_t attackType, int id, std::uint8_t direction, short x, short y, CSession* exceptSession)
{
    m_SendPacketBuffer.Clear();

    switch (attackType)
    {
    case dfPACKET_CS_ATTACK1:
        MakePacket_Attack1(m_SendPacketBuffer, id, direction, x, y);
        SendBroadcast(exceptSession, m_SendPacketBuffer);
        break;

    case dfPACKET_CS_ATTACK2:
        MakePacket_Attack2(m_SendPacketBuffer, id, direction, x, y);
        SendBroadcast(exceptSession, m_SendPacketBuffer);
        break;

    case dfPACKET_CS_ATTACK3:
        MakePacket_Attack3(m_SendPacketBuffer, id, direction, x, y);
        SendBroadcast(exceptSession, m_SendPacketBuffer);
        break;

    default:
        break;
    }
}

void CGameServer::SendDamage(int attackId, int damageId, std::uint8_t damageHp)
{
    m_SendPacketBuffer.Clear();
    MakePacket_Damage(m_SendPacketBuffer, attackId, damageId, damageHp);
    SendBroadcast(nullptr, m_SendPacketBuffer);
}
