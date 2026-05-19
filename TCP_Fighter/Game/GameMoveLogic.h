#pragma once

#include <cstdint>
#include "GameData.h"

inline bool CalcDirectionDelta(std::uint8_t direction, const MoveGameData& moveData, short& outDX, short& outDY)
{
    outDX = 0;
    outDY = 0;

    switch (direction)
    {
    case 0:
        outDX = static_cast<short>(-moveData.MoveXPerFrame);
        return true;
    case 1:
        outDX = static_cast<short>(-moveData.MoveXPerFrame);
        outDY = static_cast<short>(-moveData.MoveYPerFrame);
        return true;
    case 2:
        outDY = static_cast<short>(-moveData.MoveYPerFrame);
        return true;
    case 3:
        outDX = moveData.MoveXPerFrame;
        outDY = static_cast<short>(-moveData.MoveYPerFrame);
        return true;
    case 4:
        outDX = moveData.MoveXPerFrame;
        return true;
    case 5:
        outDX = moveData.MoveXPerFrame;
        outDY = moveData.MoveYPerFrame;
        return true;
    case 6:
        outDY = moveData.MoveYPerFrame;
        return true;
    case 7:
        outDX = static_cast<short>(-moveData.MoveXPerFrame);
        outDY = moveData.MoveYPerFrame;
        return true;
    default:
        return false;
    }
}

inline bool TryCalcNextPosition(short x, short y, std::uint8_t direction, const GameData& gameData, short& outX, short& outY)
{
    short dx = 0;
    short dy = 0;

    if (!CalcDirectionDelta(direction, gameData.Move, dx, dy))
        return false;

    const short nextX = static_cast<short>(x + dx);
    const short nextY = static_cast<short>(y + dy);

    if (nextX <= gameData.World.MoveLeft || nextX >= gameData.World.MoveRight || nextY <= gameData.World.MoveTop || nextY >= gameData.World.MoveBottom)
        return false;

    outX = nextX;
    outY = nextY;
    return true;
}
