#include "GameServer.h"
#include "PacketMaker.h"

void CGameServer::SendMoveStop(int id, std::uint8_t direction, short x, short y)
{
    CPacket packet(dfPACKET_MAX_SIZE);
    MakePacket_MoveStop(packet, id, direction, x, y);
    SendBroadcast(nullptr, packet);
}

void CGameServer::SendSync(int id, short x, short y, CSession* targetSession)
{
    if (targetSession == nullptr)
        return;

    CPacket packet(dfPACKET_MAX_SIZE);
    MakePacket_Sync(packet, id, x, y);
    SendUnicast(targetSession, packet);
}

void CGameServer::SendAttackPacket(std::uint8_t attackType, int id, std::uint8_t direction, short x, short y, CSession* exceptSession)
{
    CPacket packet(dfPACKET_MAX_SIZE);

    switch (attackType)
    {
    case dfPACKET_CS_ATTACK1:
        MakePacket_Attack1(packet, id, direction, x, y);
        SendBroadcast(exceptSession, packet);
        break;

    case dfPACKET_CS_ATTACK2:
        MakePacket_Attack2(packet, id, direction, x, y);
        SendBroadcast(exceptSession, packet);
        break;

    case dfPACKET_CS_ATTACK3:
        MakePacket_Attack3(packet, id, direction, x, y);
        SendBroadcast(exceptSession, packet);
        break;

    default:
        break;
    }
}

void CGameServer::SendDamage(int attackId, int damageId, std::uint8_t damageHp)
{
    CPacket packet(dfPACKET_MAX_SIZE);
    MakePacket_Damage(packet, attackId, damageId, damageHp);
    SendBroadcast(nullptr, packet);
}
