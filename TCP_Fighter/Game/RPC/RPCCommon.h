#pragma once

#include <cstdint>

constexpr std::uint8_t dfPACKET_CODE = static_cast<std::uint8_t>(0x89);
constexpr int dfPACKET_HEADER_SIZE = 3;
constexpr int dfPACKET_MAX_PAYLOAD_SIZE = 255;
constexpr int dfPACKET_MAX_SIZE = dfPACKET_HEADER_SIZE + dfPACKET_MAX_PAYLOAD_SIZE;
using PacketType = std::uint8_t;
using PayloadSizeType = std::uint8_t;

constexpr std::uint8_t dfPACKET_MOVE_DIR_LL = static_cast<std::uint8_t>(0);
constexpr std::uint8_t dfPACKET_MOVE_DIR_LU = static_cast<std::uint8_t>(1);
constexpr std::uint8_t dfPACKET_MOVE_DIR_UU = static_cast<std::uint8_t>(2);
constexpr std::uint8_t dfPACKET_MOVE_DIR_RU = static_cast<std::uint8_t>(3);
constexpr std::uint8_t dfPACKET_MOVE_DIR_RR = static_cast<std::uint8_t>(4);
constexpr std::uint8_t dfPACKET_MOVE_DIR_RD = static_cast<std::uint8_t>(5);
constexpr std::uint8_t dfPACKET_MOVE_DIR_DD = static_cast<std::uint8_t>(6);
constexpr std::uint8_t dfPACKET_MOVE_DIR_LD = static_cast<std::uint8_t>(7);

constexpr PacketType dfPACKET_S2C_CREATE_MY_CHARACTER = static_cast<PacketType>(0);
constexpr PacketType dfPACKET_S2C_CREATE_OTHER_CHARACTER = static_cast<PacketType>(1);
constexpr PacketType dfPACKET_S2C_DELETE_CHARACTER = static_cast<PacketType>(2);
constexpr PacketType dfPACKET_C2S_MOVE_START = static_cast<PacketType>(10);
constexpr PacketType dfPACKET_S2C_MOVE_START = static_cast<PacketType>(11);
constexpr PacketType dfPACKET_C2S_MOVE_STOP = static_cast<PacketType>(12);
constexpr PacketType dfPACKET_S2C_MOVE_STOP = static_cast<PacketType>(13);
constexpr PacketType dfPACKET_C2S_ATTACK1 = static_cast<PacketType>(20);
constexpr PacketType dfPACKET_S2C_ATTACK1 = static_cast<PacketType>(21);
constexpr PacketType dfPACKET_C2S_ATTACK2 = static_cast<PacketType>(22);
constexpr PacketType dfPACKET_S2C_ATTACK2 = static_cast<PacketType>(23);
constexpr PacketType dfPACKET_C2S_ATTACK3 = static_cast<PacketType>(24);
constexpr PacketType dfPACKET_S2C_ATTACK3 = static_cast<PacketType>(25);
constexpr PacketType dfPACKET_S2C_DAMAGE = static_cast<PacketType>(30);
constexpr PacketType dfPACKET_C2S_SYNC = static_cast<PacketType>(250);
constexpr PacketType dfPACKET_S2C_SYNC = static_cast<PacketType>(251);

constexpr PayloadSizeType dfPACKET_PAYLOAD_SIZE_S2C_CREATE_MY_CHARACTER = static_cast<PayloadSizeType>(10);
constexpr PayloadSizeType dfPACKET_PAYLOAD_SIZE_S2C_CREATE_OTHER_CHARACTER = static_cast<PayloadSizeType>(10);
constexpr PayloadSizeType dfPACKET_PAYLOAD_SIZE_S2C_DELETE_CHARACTER = static_cast<PayloadSizeType>(4);
constexpr PayloadSizeType dfPACKET_PAYLOAD_SIZE_C2S_MOVE_START = static_cast<PayloadSizeType>(5);
constexpr PayloadSizeType dfPACKET_PAYLOAD_SIZE_S2C_MOVE_START = static_cast<PayloadSizeType>(9);
constexpr PayloadSizeType dfPACKET_PAYLOAD_SIZE_C2S_MOVE_STOP = static_cast<PayloadSizeType>(5);
constexpr PayloadSizeType dfPACKET_PAYLOAD_SIZE_S2C_MOVE_STOP = static_cast<PayloadSizeType>(9);
constexpr PayloadSizeType dfPACKET_PAYLOAD_SIZE_C2S_ATTACK1 = static_cast<PayloadSizeType>(5);
constexpr PayloadSizeType dfPACKET_PAYLOAD_SIZE_S2C_ATTACK1 = static_cast<PayloadSizeType>(9);
constexpr PayloadSizeType dfPACKET_PAYLOAD_SIZE_C2S_ATTACK2 = static_cast<PayloadSizeType>(5);
constexpr PayloadSizeType dfPACKET_PAYLOAD_SIZE_S2C_ATTACK2 = static_cast<PayloadSizeType>(9);
constexpr PayloadSizeType dfPACKET_PAYLOAD_SIZE_C2S_ATTACK3 = static_cast<PayloadSizeType>(5);
constexpr PayloadSizeType dfPACKET_PAYLOAD_SIZE_S2C_ATTACK3 = static_cast<PayloadSizeType>(9);
constexpr PayloadSizeType dfPACKET_PAYLOAD_SIZE_S2C_DAMAGE = static_cast<PayloadSizeType>(9);
constexpr PayloadSizeType dfPACKET_PAYLOAD_SIZE_C2S_SYNC = static_cast<PayloadSizeType>(4);
constexpr PayloadSizeType dfPACKET_PAYLOAD_SIZE_S2C_SYNC = static_cast<PayloadSizeType>(8);
