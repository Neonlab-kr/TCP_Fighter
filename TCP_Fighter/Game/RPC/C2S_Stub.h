#pragma once

#include "RPCCommon.h"
#include "../Core/SerializationBuffer.h"
#include "../Core/Session.h"

class IC2S_Handler
{
    public:
        virtual ~IC2S_Handler() = default;

        virtual bool OnMoveStart(CSession* session, std::uint8_t direction, std::int16_t x, std::int16_t y) = 0;
        virtual bool OnMoveStop(CSession* session, std::uint8_t direction, std::int16_t x, std::int16_t y) = 0;
        virtual bool OnAttack1(CSession* session, std::uint8_t direction, std::int16_t x, std::int16_t y) = 0;
        virtual bool OnAttack2(CSession* session, std::uint8_t direction, std::int16_t x, std::int16_t y) = 0;
        virtual bool OnAttack3(CSession* session, std::uint8_t direction, std::int16_t x, std::int16_t y) = 0;
        virtual bool OnSync(CSession* session, std::int16_t x, std::int16_t y) = 0;
};

class CC2S_Stub
{
    public:
        static inline bool Dispatch(IC2S_Handler& handler, CSession* session, CPacket& packet, PacketType packetType)
        {
            switch (packetType)
            {
                case dfPACKET_C2S_MOVE_START:
                {
                    std::uint8_t direction{};
                    std::int16_t x{};
                    std::int16_t y{};

                    packet >> direction;
                    packet >> x;
                    packet >> y;
                    if (packet.IsError())
                        return false;
                    return handler.OnMoveStart(session, direction, x, y);
                }
                case dfPACKET_C2S_MOVE_STOP:
                {
                    std::uint8_t direction{};
                    std::int16_t x{};
                    std::int16_t y{};

                    packet >> direction;
                    packet >> x;
                    packet >> y;
                    if (packet.IsError())
                        return false;
                    return handler.OnMoveStop(session, direction, x, y);
                }
                case dfPACKET_C2S_ATTACK1:
                {
                    std::uint8_t direction{};
                    std::int16_t x{};
                    std::int16_t y{};

                    packet >> direction;
                    packet >> x;
                    packet >> y;
                    if (packet.IsError())
                        return false;
                    return handler.OnAttack1(session, direction, x, y);
                }
                case dfPACKET_C2S_ATTACK2:
                {
                    std::uint8_t direction{};
                    std::int16_t x{};
                    std::int16_t y{};

                    packet >> direction;
                    packet >> x;
                    packet >> y;
                    if (packet.IsError())
                        return false;
                    return handler.OnAttack2(session, direction, x, y);
                }
                case dfPACKET_C2S_ATTACK3:
                {
                    std::uint8_t direction{};
                    std::int16_t x{};
                    std::int16_t y{};

                    packet >> direction;
                    packet >> x;
                    packet >> y;
                    if (packet.IsError())
                        return false;
                    return handler.OnAttack3(session, direction, x, y);
                }
                case dfPACKET_C2S_SYNC:
                {
                    std::int16_t x{};
                    std::int16_t y{};

                    packet >> x;
                    packet >> y;
                    if (packet.IsError())
                        return false;
                    return handler.OnSync(session, x, y);
                }
                default:
                    return false;
            }
        }
};
