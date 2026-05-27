#pragma once

#include <cstdint>
#include <cstdlib>
#include "RPC/RPCCommon.h"

inline bool IsAttackHitByRange(short attackerX, short attackerY, std::uint8_t direction, short targetX, short targetY, short rangeX, short rangeY)
{
    const int diffX = static_cast<int>(targetX) - static_cast<int>(attackerX);
    const int diffY = static_cast<int>(targetY) - static_cast<int>(attackerY);

    if (std::abs(diffY) > rangeY)
        return false;

    const bool attackLeft = direction == dfPACKET_MOVE_DIR_LL || direction == dfPACKET_MOVE_DIR_LU || direction == dfPACKET_MOVE_DIR_LD;
    const bool attackRight = direction == dfPACKET_MOVE_DIR_RR || direction == dfPACKET_MOVE_DIR_RU || direction == dfPACKET_MOVE_DIR_RD;

    if (attackLeft)
        return diffX <= 0 && diffX >= -rangeX;

    if (attackRight)
        return diffX >= 0 && diffX <= rangeX;

    return false;
}
