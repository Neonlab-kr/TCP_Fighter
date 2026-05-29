#pragma once

#include "RPCCommon.h"
#include "../Core/SerializationBuffer.h"
#include "../Core/Session.h"

class IS2C_Sender
{
    public:
        virtual ~IS2C_Sender() = default;
        virtual bool SendRPCPacket(CSession* session, const CPacket& packet) = 0;
};

class CS2C_Proxy
{
    public:
        explicit CS2C_Proxy(IS2C_Sender* sender = nullptr) : m_Sender(sender) {}
        inline void SetSender(IS2C_Sender* sender) { m_Sender = sender; }

        inline bool CreateMyCharacter(CSession* session, std::int32_t id, std::uint8_t direction, std::int16_t x, std::int16_t y, std::uint8_t hp)
        {
            if (m_Sender == nullptr)
                return false;
            CPacket packet;
            packet << dfPACKET_CODE;
            packet << dfPACKET_PAYLOAD_SIZE_S2C_CREATE_MY_CHARACTER;
            packet << dfPACKET_S2C_CREATE_MY_CHARACTER;
            packet << id;
            packet << direction;
            packet << x;
            packet << y;
            packet << hp;
            return m_Sender->SendRPCPacket(session, packet);
        }

        inline bool CreateOtherCharacter(CSession* session, std::int32_t id, std::uint8_t direction, std::int16_t x, std::int16_t y, std::uint8_t hp)
        {
            if (m_Sender == nullptr)
                return false;
            CPacket packet;
            packet << dfPACKET_CODE;
            packet << dfPACKET_PAYLOAD_SIZE_S2C_CREATE_OTHER_CHARACTER;
            packet << dfPACKET_S2C_CREATE_OTHER_CHARACTER;
            packet << id;
            packet << direction;
            packet << x;
            packet << y;
            packet << hp;
            return m_Sender->SendRPCPacket(session, packet);
        }

        inline bool DeleteCharacter(CSession* session, std::int32_t id)
        {
            if (m_Sender == nullptr)
                return false;
            CPacket packet;
            packet << dfPACKET_CODE;
            packet << dfPACKET_PAYLOAD_SIZE_S2C_DELETE_CHARACTER;
            packet << dfPACKET_S2C_DELETE_CHARACTER;
            packet << id;
            return m_Sender->SendRPCPacket(session, packet);
        }

        inline bool MoveStart(CSession* session, std::int32_t id, std::uint8_t direction, std::int16_t x, std::int16_t y)
        {
            if (m_Sender == nullptr)
                return false;
            CPacket packet;
            packet << dfPACKET_CODE;
            packet << dfPACKET_PAYLOAD_SIZE_S2C_MOVE_START;
            packet << dfPACKET_S2C_MOVE_START;
            packet << id;
            packet << direction;
            packet << x;
            packet << y;
            return m_Sender->SendRPCPacket(session, packet);
        }

        inline bool MoveStop(CSession* session, std::int32_t id, std::uint8_t direction, std::int16_t x, std::int16_t y)
        {
            if (m_Sender == nullptr)
                return false;
            CPacket packet;
            packet << dfPACKET_CODE;
            packet << dfPACKET_PAYLOAD_SIZE_S2C_MOVE_STOP;
            packet << dfPACKET_S2C_MOVE_STOP;
            packet << id;
            packet << direction;
            packet << x;
            packet << y;
            return m_Sender->SendRPCPacket(session, packet);
        }

        inline bool Attack1(CSession* session, std::int32_t id, std::uint8_t direction, std::int16_t x, std::int16_t y)
        {
            if (m_Sender == nullptr)
                return false;
            CPacket packet;
            packet << dfPACKET_CODE;
            packet << dfPACKET_PAYLOAD_SIZE_S2C_ATTACK1;
            packet << dfPACKET_S2C_ATTACK1;
            packet << id;
            packet << direction;
            packet << x;
            packet << y;
            return m_Sender->SendRPCPacket(session, packet);
        }

        inline bool Attack2(CSession* session, std::int32_t id, std::uint8_t direction, std::int16_t x, std::int16_t y)
        {
            if (m_Sender == nullptr)
                return false;
            CPacket packet;
            packet << dfPACKET_CODE;
            packet << dfPACKET_PAYLOAD_SIZE_S2C_ATTACK2;
            packet << dfPACKET_S2C_ATTACK2;
            packet << id;
            packet << direction;
            packet << x;
            packet << y;
            return m_Sender->SendRPCPacket(session, packet);
        }

        inline bool Attack3(CSession* session, std::int32_t id, std::uint8_t direction, std::int16_t x, std::int16_t y)
        {
            if (m_Sender == nullptr)
                return false;
            CPacket packet;
            packet << dfPACKET_CODE;
            packet << dfPACKET_PAYLOAD_SIZE_S2C_ATTACK3;
            packet << dfPACKET_S2C_ATTACK3;
            packet << id;
            packet << direction;
            packet << x;
            packet << y;
            return m_Sender->SendRPCPacket(session, packet);
        }

        inline bool Damage(CSession* session, std::int32_t id, std::int32_t damageId, std::uint8_t damageHp)
        {
            if (m_Sender == nullptr)
                return false;
            CPacket packet;
            packet << dfPACKET_CODE;
            packet << dfPACKET_PAYLOAD_SIZE_S2C_DAMAGE;
            packet << dfPACKET_S2C_DAMAGE;
            packet << id;
            packet << damageId;
            packet << damageHp;
            return m_Sender->SendRPCPacket(session, packet);
        }

        inline bool Sync(CSession* session, std::int32_t id, std::int16_t x, std::int16_t y)
        {
            if (m_Sender == nullptr)
                return false;
            CPacket packet;
            packet << dfPACKET_CODE;
            packet << dfPACKET_PAYLOAD_SIZE_S2C_SYNC;
            packet << dfPACKET_S2C_SYNC;
            packet << id;
            packet << x;
            packet << y;
            return m_Sender->SendRPCPacket(session, packet);
        }

    private:
        IS2C_Sender* m_Sender;
};
