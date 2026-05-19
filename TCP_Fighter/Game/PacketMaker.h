#pragma once

#include "PacketDefine.h"

inline void MakePacket_CreateMyCharacter(st_PACKET_SC_CREATE_MY_CHARACTER& packet, int id, std::uint8_t direction, short x, short y, std::uint8_t hp);
inline void MakePacket_CreateOtherCharacter(st_PACKET_SC_CREATE_OTHER_CHARACTER& packet, int id, std::uint8_t direction, short x, short y, std::uint8_t hp);
inline void MakePacket_DeleteCharacter(st_PACKET_SC_DELETE_CHARACTER& packet, int id);
inline void MakePacket_MoveStart(st_PACKET_SC_MOVE_START& packet, int id, std::uint8_t direction, short x, short y);
inline void MakePacket_MoveStop(st_PACKET_SC_MOVE_STOP& packet, int id, std::uint8_t direction, short x, short y);
inline void MakePacket_Attack1(st_PACKET_SC_ATTACK1& packet, int id, std::uint8_t direction, short x, short y);
inline void MakePacket_Attack2(st_PACKET_SC_ATTACK2& packet, int id, std::uint8_t direction, short x, short y);
inline void MakePacket_Attack3(st_PACKET_SC_ATTACK3& packet, int id, std::uint8_t direction, short x, short y);
inline void MakePacket_Damage(st_PACKET_SC_DAMAGE& packet, int attackId, int damageId, std::uint8_t damageHp);
inline void MakePacket_Sync(st_PACKET_SC_SYNC& packet, int id, short x, short y);

inline void MakePacket_CreateMyCharacter(st_PACKET_SC_CREATE_MY_CHARACTER& packet, int id, std::uint8_t direction, short x, short y, std::uint8_t hp)
{
    packet.header = MakePacketHeader<st_PACKET_SC_CREATE_MY_CHARACTER>(dfPACKET_SC_CREATE_MY_CHARACTER);
    packet.id = id;
    packet.direction = direction;
    packet.x = x;
    packet.y = y;
    packet.hp = hp;
}

inline void MakePacket_CreateOtherCharacter(st_PACKET_SC_CREATE_OTHER_CHARACTER& packet, int id, std::uint8_t direction, short x, short y, std::uint8_t hp)
{
    packet.header = MakePacketHeader<st_PACKET_SC_CREATE_OTHER_CHARACTER>(dfPACKET_SC_CREATE_OTHER_CHARACTER);
    packet.id = id;
    packet.direction = direction;
    packet.x = x;
    packet.y = y;
    packet.hp = hp;
}

inline void MakePacket_DeleteCharacter(st_PACKET_SC_DELETE_CHARACTER& packet, int id)
{
    packet.header = MakePacketHeader<st_PACKET_SC_DELETE_CHARACTER>(dfPACKET_SC_DELETE_CHARACTER);
    packet.id = id;
}

inline void MakePacket_MoveStart(st_PACKET_SC_MOVE_START& packet, int id, std::uint8_t direction, short x, short y)
{
    packet.header = MakePacketHeader<st_PACKET_SC_MOVE_START>(dfPACKET_SC_MOVE_START);
    packet.id = id;
    packet.direction = direction;
    packet.x = x;
    packet.y = y;
}

inline void MakePacket_MoveStop(st_PACKET_SC_MOVE_STOP& packet, int id, std::uint8_t direction, short x, short y)
{
    packet.header = MakePacketHeader<st_PACKET_SC_MOVE_STOP>(dfPACKET_SC_MOVE_STOP);
    packet.id = id;
    packet.direction = direction;
    packet.x = x;
    packet.y = y;
}

inline void MakePacket_Attack1(st_PACKET_SC_ATTACK1& packet, int id, std::uint8_t direction, short x, short y)
{
    packet.header = MakePacketHeader<st_PACKET_SC_ATTACK1>(dfPACKET_SC_ATTACK1);
    packet.id = id;
    packet.direction = direction;
    packet.x = x;
    packet.y = y;
}

inline void MakePacket_Attack2(st_PACKET_SC_ATTACK2& packet, int id, std::uint8_t direction, short x, short y)
{
    packet.header = MakePacketHeader<st_PACKET_SC_ATTACK2>(dfPACKET_SC_ATTACK2);
    packet.id = id;
    packet.direction = direction;
    packet.x = x;
    packet.y = y;
}

inline void MakePacket_Attack3(st_PACKET_SC_ATTACK3& packet, int id, std::uint8_t direction, short x, short y)
{
    packet.header = MakePacketHeader<st_PACKET_SC_ATTACK3>(dfPACKET_SC_ATTACK3);
    packet.id = id;
    packet.direction = direction;
    packet.x = x;
    packet.y = y;
}

inline void MakePacket_Damage(st_PACKET_SC_DAMAGE& packet, int attackId, int damageId, std::uint8_t damageHp)
{
    packet.header = MakePacketHeader<st_PACKET_SC_DAMAGE>(dfPACKET_SC_DAMAGE);
    packet.attackId = attackId;
    packet.damageId = damageId;
    packet.damageHp = damageHp;
}

inline void MakePacket_Sync(st_PACKET_SC_SYNC& packet, int id, short x, short y)
{
    packet.header = MakePacketHeader<st_PACKET_SC_SYNC>(dfPACKET_SC_SYNC);
    packet.id = id;
    packet.x = x;
    packet.y = y;
}
