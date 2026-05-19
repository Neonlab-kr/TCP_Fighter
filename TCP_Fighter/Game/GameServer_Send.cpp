#include "GameServer.h"
#include "PacketMaker.h"

void CGameServer::SendMoveStop(int id, std::uint8_t direction, short x, short y)
{
    st_PACKET_SC_MOVE_STOP packet{};
    MakePacket_MoveStop(packet, id, direction, x, y);
    SendBroadcast(nullptr, packet);
}

void CGameServer::SendSync(int id, short x, short y, CSession* targetSession)
{
    if (targetSession == nullptr)
        return;

    st_PACKET_SC_SYNC packet{};
    MakePacket_Sync(packet, id, x, y);
    SendUnicast(targetSession, packet);
}

void CGameServer::SendAttackPacket(std::uint8_t attackType, int id, std::uint8_t direction, short x, short y, CSession* exceptSession)
{
    switch (attackType)
    {
    case dfPACKET_CS_ATTACK1:
    {
        st_PACKET_SC_ATTACK1 packet{};
        MakePacket_Attack1(packet, id, direction, x, y);
        SendBroadcast(exceptSession, packet);
        break;
    }

    case dfPACKET_CS_ATTACK2:
    {
        st_PACKET_SC_ATTACK2 packet{};
        MakePacket_Attack2(packet, id, direction, x, y);
        SendBroadcast(exceptSession, packet);
        break;
    }

    case dfPACKET_CS_ATTACK3:
    {
        st_PACKET_SC_ATTACK3 packet{};
        MakePacket_Attack3(packet, id, direction, x, y);
        SendBroadcast(exceptSession, packet);
        break;
    }

    default:
        break;
    }
}

void CGameServer::SendDamage(int attackId, int damageId, std::uint8_t damageHp)
{
    st_PACKET_SC_DAMAGE packet{};
    MakePacket_Damage(packet, attackId, damageId, damageHp);
    SendBroadcast(nullptr, packet);
}
