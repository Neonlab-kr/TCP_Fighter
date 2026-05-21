#pragma once

#include <cstdint>
#include "PacketDefine.h"
#include "../Core/SerializationBuffer.h"

inline void WritePacketHeader(CPacket& packet, std::uint8_t packetType, std::uint8_t payloadSize)
{
    packet << dfPACKET_CODE;
    packet << payloadSize;
    packet << packetType;
}

inline void MakePacket_SC_CREATE_MY_CHARACTER(CPacket& packet, std::int32_t id, std::uint8_t direction, std::int16_t x, std::int16_t y, std::uint8_t hp)
{
    packet.Clear();
    WritePacketHeader(packet, dfPACKET_SC_CREATE_MY_CHARACTER, dfPACKET_PAYLOAD_SIZE_SC_CREATE_MY_CHARACTER);
    packet << id;
    packet << direction;
    packet << x;
    packet << y;
    packet << hp;
}

inline bool ReadPayload_SC_CREATE_MY_CHARACTER(CPacket& packet, std::int32_t& id, std::uint8_t& direction, std::int16_t& x, std::int16_t& y, std::uint8_t& hp)
{
    packet >> id;
    packet >> direction;
    packet >> x;
    packet >> y;
    packet >> hp;
    return !packet.IsError();
}

inline void MakePacket_SC_CREATE_OTHER_CHARACTER(CPacket& packet, std::int32_t id, std::uint8_t direction, std::int16_t x, std::int16_t y, std::uint8_t hp)
{
    packet.Clear();
    WritePacketHeader(packet, dfPACKET_SC_CREATE_OTHER_CHARACTER, dfPACKET_PAYLOAD_SIZE_SC_CREATE_OTHER_CHARACTER);
    packet << id;
    packet << direction;
    packet << x;
    packet << y;
    packet << hp;
}

inline bool ReadPayload_SC_CREATE_OTHER_CHARACTER(CPacket& packet, std::int32_t& id, std::uint8_t& direction, std::int16_t& x, std::int16_t& y, std::uint8_t& hp)
{
    packet >> id;
    packet >> direction;
    packet >> x;
    packet >> y;
    packet >> hp;
    return !packet.IsError();
}

inline void MakePacket_SC_DELETE_CHARACTER(CPacket& packet, std::int32_t id)
{
    packet.Clear();
    WritePacketHeader(packet, dfPACKET_SC_DELETE_CHARACTER, dfPACKET_PAYLOAD_SIZE_SC_DELETE_CHARACTER);
    packet << id;
}

inline bool ReadPayload_SC_DELETE_CHARACTER(CPacket& packet, std::int32_t& id)
{
    packet >> id;
    return !packet.IsError();
}

inline void MakePacket_CS_MOVE_START(CPacket& packet, std::uint8_t direction, std::int16_t x, std::int16_t y)
{
    packet.Clear();
    WritePacketHeader(packet, dfPACKET_CS_MOVE_START, dfPACKET_PAYLOAD_SIZE_CS_MOVE_START);
    packet << direction;
    packet << x;
    packet << y;
}

inline bool ReadPayload_CS_MOVE_START(CPacket& packet, std::uint8_t& direction, std::int16_t& x, std::int16_t& y)
{
    packet >> direction;
    packet >> x;
    packet >> y;
    return !packet.IsError();
}

inline void MakePacket_SC_MOVE_START(CPacket& packet, std::int32_t id, std::uint8_t direction, std::int16_t x, std::int16_t y)
{
    packet.Clear();
    WritePacketHeader(packet, dfPACKET_SC_MOVE_START, dfPACKET_PAYLOAD_SIZE_SC_MOVE_START);
    packet << id;
    packet << direction;
    packet << x;
    packet << y;
}

inline bool ReadPayload_SC_MOVE_START(CPacket& packet, std::int32_t& id, std::uint8_t& direction, std::int16_t& x, std::int16_t& y)
{
    packet >> id;
    packet >> direction;
    packet >> x;
    packet >> y;
    return !packet.IsError();
}

inline void MakePacket_CS_MOVE_STOP(CPacket& packet, std::uint8_t direction, std::int16_t x, std::int16_t y)
{
    packet.Clear();
    WritePacketHeader(packet, dfPACKET_CS_MOVE_STOP, dfPACKET_PAYLOAD_SIZE_CS_MOVE_STOP);
    packet << direction;
    packet << x;
    packet << y;
}

inline bool ReadPayload_CS_MOVE_STOP(CPacket& packet, std::uint8_t& direction, std::int16_t& x, std::int16_t& y)
{
    packet >> direction;
    packet >> x;
    packet >> y;
    return !packet.IsError();
}

inline void MakePacket_SC_MOVE_STOP(CPacket& packet, std::int32_t id, std::uint8_t direction, std::int16_t x, std::int16_t y)
{
    packet.Clear();
    WritePacketHeader(packet, dfPACKET_SC_MOVE_STOP, dfPACKET_PAYLOAD_SIZE_SC_MOVE_STOP);
    packet << id;
    packet << direction;
    packet << x;
    packet << y;
}

inline bool ReadPayload_SC_MOVE_STOP(CPacket& packet, std::int32_t& id, std::uint8_t& direction, std::int16_t& x, std::int16_t& y)
{
    packet >> id;
    packet >> direction;
    packet >> x;
    packet >> y;
    return !packet.IsError();
}

inline void MakePacket_CS_ATTACK1(CPacket& packet, std::uint8_t direction, std::int16_t x, std::int16_t y)
{
    packet.Clear();
    WritePacketHeader(packet, dfPACKET_CS_ATTACK1, dfPACKET_PAYLOAD_SIZE_CS_ATTACK1);
    packet << direction;
    packet << x;
    packet << y;
}

inline bool ReadPayload_CS_ATTACK1(CPacket& packet, std::uint8_t& direction, std::int16_t& x, std::int16_t& y)
{
    packet >> direction;
    packet >> x;
    packet >> y;
    return !packet.IsError();
}

inline void MakePacket_SC_ATTACK1(CPacket& packet, std::int32_t id, std::uint8_t direction, std::int16_t x, std::int16_t y)
{
    packet.Clear();
    WritePacketHeader(packet, dfPACKET_SC_ATTACK1, dfPACKET_PAYLOAD_SIZE_SC_ATTACK1);
    packet << id;
    packet << direction;
    packet << x;
    packet << y;
}

inline bool ReadPayload_SC_ATTACK1(CPacket& packet, std::int32_t& id, std::uint8_t& direction, std::int16_t& x, std::int16_t& y)
{
    packet >> id;
    packet >> direction;
    packet >> x;
    packet >> y;
    return !packet.IsError();
}

inline void MakePacket_CS_ATTACK2(CPacket& packet, std::uint8_t direction, std::int16_t x, std::int16_t y)
{
    packet.Clear();
    WritePacketHeader(packet, dfPACKET_CS_ATTACK2, dfPACKET_PAYLOAD_SIZE_CS_ATTACK2);
    packet << direction;
    packet << x;
    packet << y;
}

inline bool ReadPayload_CS_ATTACK2(CPacket& packet, std::uint8_t& direction, std::int16_t& x, std::int16_t& y)
{
    packet >> direction;
    packet >> x;
    packet >> y;
    return !packet.IsError();
}

inline void MakePacket_SC_ATTACK2(CPacket& packet, std::int32_t id, std::uint8_t direction, std::int16_t x, std::int16_t y)
{
    packet.Clear();
    WritePacketHeader(packet, dfPACKET_SC_ATTACK2, dfPACKET_PAYLOAD_SIZE_SC_ATTACK2);
    packet << id;
    packet << direction;
    packet << x;
    packet << y;
}

inline bool ReadPayload_SC_ATTACK2(CPacket& packet, std::int32_t& id, std::uint8_t& direction, std::int16_t& x, std::int16_t& y)
{
    packet >> id;
    packet >> direction;
    packet >> x;
    packet >> y;
    return !packet.IsError();
}

inline void MakePacket_CS_ATTACK3(CPacket& packet, std::uint8_t direction, std::int16_t x, std::int16_t y)
{
    packet.Clear();
    WritePacketHeader(packet, dfPACKET_CS_ATTACK3, dfPACKET_PAYLOAD_SIZE_CS_ATTACK3);
    packet << direction;
    packet << x;
    packet << y;
}

inline bool ReadPayload_CS_ATTACK3(CPacket& packet, std::uint8_t& direction, std::int16_t& x, std::int16_t& y)
{
    packet >> direction;
    packet >> x;
    packet >> y;
    return !packet.IsError();
}

inline void MakePacket_SC_ATTACK3(CPacket& packet, std::int32_t id, std::uint8_t direction, std::int16_t x, std::int16_t y)
{
    packet.Clear();
    WritePacketHeader(packet, dfPACKET_SC_ATTACK3, dfPACKET_PAYLOAD_SIZE_SC_ATTACK3);
    packet << id;
    packet << direction;
    packet << x;
    packet << y;
}

inline bool ReadPayload_SC_ATTACK3(CPacket& packet, std::int32_t& id, std::uint8_t& direction, std::int16_t& x, std::int16_t& y)
{
    packet >> id;
    packet >> direction;
    packet >> x;
    packet >> y;
    return !packet.IsError();
}

inline void MakePacket_SC_DAMAGE(CPacket& packet, std::int32_t attackId, std::int32_t damageId, std::uint8_t damageHp)
{
    packet.Clear();
    WritePacketHeader(packet, dfPACKET_SC_DAMAGE, dfPACKET_PAYLOAD_SIZE_SC_DAMAGE);
    packet << attackId;
    packet << damageId;
    packet << damageHp;
}

inline bool ReadPayload_SC_DAMAGE(CPacket& packet, std::int32_t& attackId, std::int32_t& damageId, std::uint8_t& damageHp)
{
    packet >> attackId;
    packet >> damageId;
    packet >> damageHp;
    return !packet.IsError();
}

inline void MakePacket_CS_SYNC(CPacket& packet, std::int16_t x, std::int16_t y)
{
    packet.Clear();
    WritePacketHeader(packet, dfPACKET_CS_SYNC, dfPACKET_PAYLOAD_SIZE_CS_SYNC);
    packet << x;
    packet << y;
}

inline bool ReadPayload_CS_SYNC(CPacket& packet, std::int16_t& x, std::int16_t& y)
{
    packet >> x;
    packet >> y;
    return !packet.IsError();
}

inline void MakePacket_SC_SYNC(CPacket& packet, std::int32_t id, std::int16_t x, std::int16_t y)
{
    packet.Clear();
    WritePacketHeader(packet, dfPACKET_SC_SYNC, dfPACKET_PAYLOAD_SIZE_SC_SYNC);
    packet << id;
    packet << x;
    packet << y;
}

inline bool ReadPayload_SC_SYNC(CPacket& packet, std::int32_t& id, std::int16_t& x, std::int16_t& y)
{
    packet >> id;
    packet >> x;
    packet >> y;
    return !packet.IsError();
}

