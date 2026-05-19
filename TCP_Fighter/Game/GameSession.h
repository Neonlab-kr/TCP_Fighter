#pragma once

#include <cstdint>

class CSession;

struct GameSession
{
    CSession* NetSession = nullptr;
    std::uint32_t AttackLockUntilTimeMs = 0;
    int PlayerId = -1;
    short X = 0;
    short Y = 0;
    std::uint8_t Direction = 0;
    std::uint8_t HP = 100;
    bool Moving = false;
    bool Active = false;
};
