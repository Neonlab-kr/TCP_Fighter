#pragma once

#include <cstdint>
#include <type_traits>

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

#pragma pack(push, 1)

struct st_PACKET_HEADER
{
    std::uint8_t byCode;
    std::uint8_t bySize;
    std::uint8_t byType;
};

struct st_PACKET_SC_CREATE_MY_CHARACTER
{
    st_PACKET_HEADER header;
    int id;
    std::uint8_t direction;
    short x;
    short y;
    std::uint8_t hp;
};

struct st_PACKET_SC_CREATE_OTHER_CHARACTER
{
    st_PACKET_HEADER header;
    int id;
    std::uint8_t direction;
    short x;
    short y;
    std::uint8_t hp;
};

struct st_PACKET_SC_DELETE_CHARACTER
{
    st_PACKET_HEADER header;
    int id;
};

struct st_PACKET_CS_MOVE_START
{
    st_PACKET_HEADER header;
    std::uint8_t direction;
    short x;
    short y;
};

struct st_PACKET_SC_MOVE_START
{
    st_PACKET_HEADER header;
    int id;
    std::uint8_t direction;
    short x;
    short y;
};

struct st_PACKET_CS_MOVE_STOP
{
    st_PACKET_HEADER header;
    std::uint8_t direction;
    short x;
    short y;
};

struct st_PACKET_SC_MOVE_STOP
{
    st_PACKET_HEADER header;
    int id;
    std::uint8_t direction;
    short x;
    short y;
};

struct st_PACKET_CS_ATTACK1
{
    st_PACKET_HEADER header;
    std::uint8_t direction;
    short x;
    short y;
};

struct st_PACKET_SC_ATTACK1
{
    st_PACKET_HEADER header;
    int id;
    std::uint8_t direction;
    short x;
    short y;
};

struct st_PACKET_CS_ATTACK2
{
    st_PACKET_HEADER header;
    std::uint8_t direction;
    short x;
    short y;
};

struct st_PACKET_SC_ATTACK2
{
    st_PACKET_HEADER header;
    int id;
    std::uint8_t direction;
    short x;
    short y;
};

struct st_PACKET_CS_ATTACK3
{
    st_PACKET_HEADER header;
    std::uint8_t direction;
    short x;
    short y;
};

struct st_PACKET_SC_ATTACK3
{
    st_PACKET_HEADER header;
    int id;
    std::uint8_t direction;
    short x;
    short y;
};

struct st_PACKET_SC_DAMAGE
{
    st_PACKET_HEADER header;
    int attackId;
    int damageId;
    std::uint8_t damageHp;
};

struct st_PACKET_CS_SYNC
{
    st_PACKET_HEADER header;
    short x;
    short y;
};

struct st_PACKET_SC_SYNC
{
    st_PACKET_HEADER header;
    int id;
    short x;
    short y;
};

#pragma pack(pop)

static_assert(sizeof(st_PACKET_HEADER) == dfPACKET_HEADER_SIZE, "packet header size must be 3");
static_assert(sizeof(st_PACKET_SC_CREATE_MY_CHARACTER) <= dfPACKET_MAX_SIZE, "packet size overflow");
static_assert(sizeof(st_PACKET_SC_CREATE_OTHER_CHARACTER) <= dfPACKET_MAX_SIZE, "packet size overflow");
static_assert(sizeof(st_PACKET_SC_DELETE_CHARACTER) <= dfPACKET_MAX_SIZE, "packet size overflow");
static_assert(sizeof(st_PACKET_CS_MOVE_START) <= dfPACKET_MAX_SIZE, "packet size overflow");
static_assert(sizeof(st_PACKET_SC_MOVE_START) <= dfPACKET_MAX_SIZE, "packet size overflow");
static_assert(sizeof(st_PACKET_CS_MOVE_STOP) <= dfPACKET_MAX_SIZE, "packet size overflow");
static_assert(sizeof(st_PACKET_SC_MOVE_STOP) <= dfPACKET_MAX_SIZE, "packet size overflow");
static_assert(sizeof(st_PACKET_CS_ATTACK1) <= dfPACKET_MAX_SIZE, "packet size overflow");
static_assert(sizeof(st_PACKET_SC_ATTACK1) <= dfPACKET_MAX_SIZE, "packet size overflow");
static_assert(sizeof(st_PACKET_CS_ATTACK2) <= dfPACKET_MAX_SIZE, "packet size overflow");
static_assert(sizeof(st_PACKET_SC_ATTACK2) <= dfPACKET_MAX_SIZE, "packet size overflow");
static_assert(sizeof(st_PACKET_CS_ATTACK3) <= dfPACKET_MAX_SIZE, "packet size overflow");
static_assert(sizeof(st_PACKET_SC_ATTACK3) <= dfPACKET_MAX_SIZE, "packet size overflow");
static_assert(sizeof(st_PACKET_SC_DAMAGE) <= dfPACKET_MAX_SIZE, "packet size overflow");
static_assert(sizeof(st_PACKET_CS_SYNC) <= dfPACKET_MAX_SIZE, "packet size overflow");
static_assert(sizeof(st_PACKET_SC_SYNC) <= dfPACKET_MAX_SIZE, "packet size overflow");

template <typename TPacket>
inline st_PACKET_HEADER MakePacketHeader(std::uint8_t type)
{
    static_assert(std::is_trivially_copyable<TPacket>::value, "packet must be trivially copyable");
    static_assert(sizeof(TPacket) >= sizeof(st_PACKET_HEADER), "packet must include header");
    static_assert(sizeof(TPacket) <= dfPACKET_MAX_SIZE, "packet size overflow");

    st_PACKET_HEADER header{};
    header.byCode = dfPACKET_CODE;
    header.bySize = static_cast<std::uint8_t>(sizeof(TPacket) - sizeof(st_PACKET_HEADER));
    header.byType = type;
    return header;
}
