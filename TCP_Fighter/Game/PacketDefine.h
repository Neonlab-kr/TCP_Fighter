#pragma once

#include <cstdint>

constexpr std::uint8_t dfPACKET_CODE = 0x89;
constexpr int dfPACKET_HEADER_SIZE = 3;
constexpr int dfPACKET_MAX_SIZE = 255 + dfPACKET_HEADER_SIZE;

#define dfPACKET_SC_CREATE_MY_CHARACTER     0
#define dfPACKET_SC_CREATE_OTHER_CHARACTER  1
#define dfPACKET_SC_DELETE_CHARACTER        2

#define dfPACKET_CS_MOVE_START              10
#define dfPACKET_SC_MOVE_START              11
#define dfPACKET_CS_MOVE_STOP               12
#define dfPACKET_SC_MOVE_STOP               13

#define dfPACKET_CS_ATTACK1                 20
#define dfPACKET_SC_ATTACK1                 21
#define dfPACKET_CS_ATTACK2                 22
#define dfPACKET_SC_ATTACK2                 23
#define dfPACKET_CS_ATTACK3                 24
#define dfPACKET_SC_ATTACK3                 25

#define dfPACKET_SC_DAMAGE                  30

#define dfPACKET_CS_SYNC                    250
#define dfPACKET_SC_SYNC                    251

#define dfPACKET_MOVE_DIR_LL                0
#define dfPACKET_MOVE_DIR_LU                1
#define dfPACKET_MOVE_DIR_UU                2
#define dfPACKET_MOVE_DIR_RU                3
#define dfPACKET_MOVE_DIR_RR                4
#define dfPACKET_MOVE_DIR_RD                5
#define dfPACKET_MOVE_DIR_DD                6
#define dfPACKET_MOVE_DIR_LD                7

constexpr int dfPACKET_SIZE_SC_CREATE_MY_CHARACTER = sizeof(int) + sizeof(std::uint8_t) + sizeof(short) + sizeof(short) + sizeof(std::uint8_t);
constexpr int dfPACKET_SIZE_SC_CREATE_OTHER_CHARACTER = sizeof(int) + sizeof(std::uint8_t) + sizeof(short) + sizeof(short) + sizeof(std::uint8_t);
constexpr int dfPACKET_SIZE_SC_DELETE_CHARACTER = sizeof(int);

constexpr int dfPACKET_SIZE_CS_MOVE_START = sizeof(std::uint8_t) + sizeof(short) + sizeof(short);
constexpr int dfPACKET_SIZE_SC_MOVE_START = sizeof(int) + sizeof(std::uint8_t) + sizeof(short) + sizeof(short);
constexpr int dfPACKET_SIZE_CS_MOVE_STOP = sizeof(std::uint8_t) + sizeof(short) + sizeof(short);
constexpr int dfPACKET_SIZE_SC_MOVE_STOP = sizeof(int) + sizeof(std::uint8_t) + sizeof(short) + sizeof(short);

constexpr int dfPACKET_SIZE_CS_ATTACK1 = sizeof(std::uint8_t) + sizeof(short) + sizeof(short);
constexpr int dfPACKET_SIZE_SC_ATTACK1 = sizeof(int) + sizeof(std::uint8_t) + sizeof(short) + sizeof(short);
constexpr int dfPACKET_SIZE_CS_ATTACK2 = sizeof(std::uint8_t) + sizeof(short) + sizeof(short);
constexpr int dfPACKET_SIZE_SC_ATTACK2 = sizeof(int) + sizeof(std::uint8_t) + sizeof(short) + sizeof(short);
constexpr int dfPACKET_SIZE_CS_ATTACK3 = sizeof(std::uint8_t) + sizeof(short) + sizeof(short);
constexpr int dfPACKET_SIZE_SC_ATTACK3 = sizeof(int) + sizeof(std::uint8_t) + sizeof(short) + sizeof(short);

constexpr int dfPACKET_SIZE_SC_DAMAGE = sizeof(int) + sizeof(int) + sizeof(std::uint8_t);
constexpr int dfPACKET_SIZE_CS_SYNC = sizeof(short) + sizeof(short);
constexpr int dfPACKET_SIZE_SC_SYNC = sizeof(int) + sizeof(short) + sizeof(short);

