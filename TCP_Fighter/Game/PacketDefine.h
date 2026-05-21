#pragma once

#include <cstdint>

constexpr std::uint8_t dfPACKET_CODE = static_cast<std::uint8_t>(0x89);
constexpr int dfPACKET_HEADER_SIZE = 3;
constexpr int dfPACKET_MAX_PAYLOAD_SIZE = 255;
constexpr int dfPACKET_MAX_SIZE = dfPACKET_HEADER_SIZE + dfPACKET_MAX_PAYLOAD_SIZE;

constexpr std::uint8_t dfPACKET_MOVE_DIR_LL = 0;
constexpr std::uint8_t dfPACKET_MOVE_DIR_LU = 1;
constexpr std::uint8_t dfPACKET_MOVE_DIR_UU = 2;
constexpr std::uint8_t dfPACKET_MOVE_DIR_RU = 3;
constexpr std::uint8_t dfPACKET_MOVE_DIR_RR = 4;
constexpr std::uint8_t dfPACKET_MOVE_DIR_RD = 5;
constexpr std::uint8_t dfPACKET_MOVE_DIR_DD = 6;
constexpr std::uint8_t dfPACKET_MOVE_DIR_LD = 7;

constexpr std::uint8_t dfPACKET_SC_CREATE_MY_CHARACTER = static_cast<std::uint8_t>(0);
constexpr std::uint8_t dfPACKET_SC_CREATE_OTHER_CHARACTER = static_cast<std::uint8_t>(1);
constexpr std::uint8_t dfPACKET_SC_DELETE_CHARACTER = static_cast<std::uint8_t>(2);
constexpr std::uint8_t dfPACKET_CS_MOVE_START = static_cast<std::uint8_t>(10);
constexpr std::uint8_t dfPACKET_SC_MOVE_START = static_cast<std::uint8_t>(11);
constexpr std::uint8_t dfPACKET_CS_MOVE_STOP = static_cast<std::uint8_t>(12);
constexpr std::uint8_t dfPACKET_SC_MOVE_STOP = static_cast<std::uint8_t>(13);
constexpr std::uint8_t dfPACKET_CS_ATTACK1 = static_cast<std::uint8_t>(20);
constexpr std::uint8_t dfPACKET_SC_ATTACK1 = static_cast<std::uint8_t>(21);
constexpr std::uint8_t dfPACKET_CS_ATTACK2 = static_cast<std::uint8_t>(22);
constexpr std::uint8_t dfPACKET_SC_ATTACK2 = static_cast<std::uint8_t>(23);
constexpr std::uint8_t dfPACKET_CS_ATTACK3 = static_cast<std::uint8_t>(24);
constexpr std::uint8_t dfPACKET_SC_ATTACK3 = static_cast<std::uint8_t>(25);
constexpr std::uint8_t dfPACKET_SC_DAMAGE = static_cast<std::uint8_t>(30);
constexpr std::uint8_t dfPACKET_CS_SYNC = static_cast<std::uint8_t>(250);
constexpr std::uint8_t dfPACKET_SC_SYNC = static_cast<std::uint8_t>(251);

constexpr std::uint8_t dfPACKET_PAYLOAD_SIZE_SC_CREATE_MY_CHARACTER = static_cast<std::uint8_t>(10);
constexpr std::uint8_t dfPACKET_PAYLOAD_SIZE_SC_CREATE_OTHER_CHARACTER = static_cast<std::uint8_t>(10);
constexpr std::uint8_t dfPACKET_PAYLOAD_SIZE_SC_DELETE_CHARACTER = static_cast<std::uint8_t>(4);
constexpr std::uint8_t dfPACKET_PAYLOAD_SIZE_CS_MOVE_START = static_cast<std::uint8_t>(5);
constexpr std::uint8_t dfPACKET_PAYLOAD_SIZE_SC_MOVE_START = static_cast<std::uint8_t>(9);
constexpr std::uint8_t dfPACKET_PAYLOAD_SIZE_CS_MOVE_STOP = static_cast<std::uint8_t>(5);
constexpr std::uint8_t dfPACKET_PAYLOAD_SIZE_SC_MOVE_STOP = static_cast<std::uint8_t>(9);
constexpr std::uint8_t dfPACKET_PAYLOAD_SIZE_CS_ATTACK1 = static_cast<std::uint8_t>(5);
constexpr std::uint8_t dfPACKET_PAYLOAD_SIZE_SC_ATTACK1 = static_cast<std::uint8_t>(9);
constexpr std::uint8_t dfPACKET_PAYLOAD_SIZE_CS_ATTACK2 = static_cast<std::uint8_t>(5);
constexpr std::uint8_t dfPACKET_PAYLOAD_SIZE_SC_ATTACK2 = static_cast<std::uint8_t>(9);
constexpr std::uint8_t dfPACKET_PAYLOAD_SIZE_CS_ATTACK3 = static_cast<std::uint8_t>(5);
constexpr std::uint8_t dfPACKET_PAYLOAD_SIZE_SC_ATTACK3 = static_cast<std::uint8_t>(9);
constexpr std::uint8_t dfPACKET_PAYLOAD_SIZE_SC_DAMAGE = static_cast<std::uint8_t>(9);
constexpr std::uint8_t dfPACKET_PAYLOAD_SIZE_CS_SYNC = static_cast<std::uint8_t>(4);
constexpr std::uint8_t dfPACKET_PAYLOAD_SIZE_SC_SYNC = static_cast<std::uint8_t>(8);
