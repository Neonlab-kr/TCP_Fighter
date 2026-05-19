#pragma once

#include "PacketDefine.h"
#include "../Core/SerializationBuffer.h"

inline void MakePacketHeader(CPacket& packet, std::uint8_t type, std::uint8_t payloadSize)
{
    packet.Clear();
    packet << dfPACKET_CODE;
    packet << payloadSize;
    packet << type;
}

inline void MakePacket_CreateMyCharacter(CPacket& packet, int id, std::uint8_t direction, short x, short y, std::uint8_t hp)
{
    MakePacketHeader(packet, dfPACKET_SC_CREATE_MY_CHARACTER, static_cast<std::uint8_t>(dfPACKET_SIZE_SC_CREATE_MY_CHARACTER));
    packet << id << direction << x << y << hp;
}

inline void MakePacket_CreateOtherCharacter(CPacket& packet, int id, std::uint8_t direction, short x, short y, std::uint8_t hp)
{
    MakePacketHeader(packet, dfPACKET_SC_CREATE_OTHER_CHARACTER, static_cast<std::uint8_t>(dfPACKET_SIZE_SC_CREATE_OTHER_CHARACTER));
    packet << id << direction << x << y << hp;
}

inline void MakePacket_DeleteCharacter(CPacket& packet, int id)
{
    MakePacketHeader(packet, dfPACKET_SC_DELETE_CHARACTER, static_cast<std::uint8_t>(dfPACKET_SIZE_SC_DELETE_CHARACTER));
    packet << id;
}

inline void MakePacket_MoveStart(CPacket& packet, int id, std::uint8_t direction, short x, short y)
{
    MakePacketHeader(packet, dfPACKET_SC_MOVE_START, static_cast<std::uint8_t>(dfPACKET_SIZE_SC_MOVE_START));
    packet << id << direction << x << y;
}

inline void MakePacket_MoveStop(CPacket& packet, int id, std::uint8_t direction, short x, short y)
{
    MakePacketHeader(packet, dfPACKET_SC_MOVE_STOP, static_cast<std::uint8_t>(dfPACKET_SIZE_SC_MOVE_STOP));
    packet << id << direction << x << y;
}

inline void MakePacket_Attack1(CPacket& packet, int id, std::uint8_t direction, short x, short y)
{
    MakePacketHeader(packet, dfPACKET_SC_ATTACK1, static_cast<std::uint8_t>(dfPACKET_SIZE_SC_ATTACK1));
    packet << id << direction << x << y;
}

inline void MakePacket_Attack2(CPacket& packet, int id, std::uint8_t direction, short x, short y)
{
    MakePacketHeader(packet, dfPACKET_SC_ATTACK2, static_cast<std::uint8_t>(dfPACKET_SIZE_SC_ATTACK2));
    packet << id << direction << x << y;
}

inline void MakePacket_Attack3(CPacket& packet, int id, std::uint8_t direction, short x, short y)
{
    MakePacketHeader(packet, dfPACKET_SC_ATTACK3, static_cast<std::uint8_t>(dfPACKET_SIZE_SC_ATTACK3));
    packet << id << direction << x << y;
}

inline void MakePacket_Damage(CPacket& packet, int attackId, int damageId, std::uint8_t damageHp)
{
    MakePacketHeader(packet, dfPACKET_SC_DAMAGE, static_cast<std::uint8_t>(dfPACKET_SIZE_SC_DAMAGE));
    packet << attackId << damageId << damageHp;
}

inline void MakePacket_Sync(CPacket& packet, int id, short x, short y)
{
    MakePacketHeader(packet, dfPACKET_SC_SYNC, static_cast<std::uint8_t>(dfPACKET_SIZE_SC_SYNC));
    packet << id << x << y;
}
