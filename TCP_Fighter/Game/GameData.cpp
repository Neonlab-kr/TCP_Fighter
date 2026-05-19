#include "GameData.h"
#include "../Core/IniReader.h"

namespace
{
    inline void NormalizeAttackGameData(AttackGameData& attackData)
    {
        if (attackData.RangeX < 0)
            attackData.RangeX = 0;

        if (attackData.RangeY < 0)
            attackData.RangeY = 0;

        if (attackData.CooldownMs == 0)
            attackData.CooldownMs = 1;
    }

    inline void NormalizeGameData(GameData& data)
    {
        if (data.World.ScreenWidth <= 0)
            data.World.ScreenWidth = 640;

        if (data.World.ScreenHeight <= 0)
            data.World.ScreenHeight = 480;

        if (data.World.MoveLeft < 0)
            data.World.MoveLeft = 0;

        if (data.World.MoveTop < 0)
            data.World.MoveTop = 0;

        if (data.World.MoveRight <= data.World.MoveLeft)
            data.World.MoveRight = static_cast<short>(data.World.MoveLeft + 1);

        if (data.World.MoveBottom <= data.World.MoveTop)
            data.World.MoveBottom = static_cast<short>(data.World.MoveTop + 1);

        if (data.Move.MoveXPerFrame <= 0)
            data.Move.MoveXPerFrame = 1;

        if (data.Move.MoveYPerFrame <= 0)
            data.Move.MoveYPerFrame = 1;

        if (data.Move.ErrorRange < 0)
            data.Move.ErrorRange = 0;

        if (data.Character.DefaultHP == 0)
            data.Character.DefaultHP = 1;

        NormalizeAttackGameData(data.Attack1);
        NormalizeAttackGameData(data.Attack2);
        NormalizeAttackGameData(data.Attack3);

        if (data.ServerDebug.RenderPlayerLineMax < 1)
            data.ServerDebug.RenderPlayerLineMax = 1;

        if (data.ServerDebug.RenderClearExtraLineCount < 0)
            data.ServerDebug.RenderClearExtraLineCount = 0;

        if (data.ServerDebug.MaxFixedDeltaMs == 0)
            data.ServerDebug.MaxFixedDeltaMs = 1;
    }
}

bool CGameDataLoader::LoadGameData(const char* filePath, GameData& outData)
{
    CIniReader ini;
    if (!ini.Load(filePath))
        return false;

    GameData loadedData = outData;

    loadedData.World.ScreenWidth = ini.GetShort("World", "ScreenWidth", loadedData.World.ScreenWidth);
    loadedData.World.ScreenHeight = ini.GetShort("World", "ScreenHeight", loadedData.World.ScreenHeight);
    loadedData.World.MoveTop = ini.GetShort("World", "MoveTop", loadedData.World.MoveTop);
    loadedData.World.MoveLeft = ini.GetShort("World", "MoveLeft", loadedData.World.MoveLeft);
    loadedData.World.MoveRight = ini.GetShort("World", "MoveRight", loadedData.World.MoveRight);
    loadedData.World.MoveBottom = ini.GetShort("World", "MoveBottom", loadedData.World.MoveBottom);

    loadedData.Move.MoveXPerFrame = ini.GetShort("Move", "MoveXPerFrame", loadedData.Move.MoveXPerFrame);
    loadedData.Move.MoveYPerFrame = ini.GetShort("Move", "MoveYPerFrame", loadedData.Move.MoveYPerFrame);
    loadedData.Move.ErrorRange = ini.GetShort("Move", "ErrorRange", loadedData.Move.ErrorRange);

    loadedData.Character.DefaultHP = ini.GetByte("Character", "DefaultHP", loadedData.Character.DefaultHP);

    loadedData.Attack.InputAdvanceMs = ini.GetUInt32("Attack", "InputAdvanceMs", loadedData.Attack.InputAdvanceMs);

    loadedData.Attack1.RangeX = ini.GetShort("Attack1", "RangeX", loadedData.Attack1.RangeX);
    loadedData.Attack1.RangeY = ini.GetShort("Attack1", "RangeY", loadedData.Attack1.RangeY);
    loadedData.Attack1.Damage = ini.GetByte("Attack1", "Damage", loadedData.Attack1.Damage);
    loadedData.Attack1.CooldownMs = ini.GetUInt32("Attack1", "CooldownMs", loadedData.Attack1.CooldownMs);

    loadedData.Attack2.RangeX = ini.GetShort("Attack2", "RangeX", loadedData.Attack2.RangeX);
    loadedData.Attack2.RangeY = ini.GetShort("Attack2", "RangeY", loadedData.Attack2.RangeY);
    loadedData.Attack2.Damage = ini.GetByte("Attack2", "Damage", loadedData.Attack2.Damage);
    loadedData.Attack2.CooldownMs = ini.GetUInt32("Attack2", "CooldownMs", loadedData.Attack2.CooldownMs);

    loadedData.Attack3.RangeX = ini.GetShort("Attack3", "RangeX", loadedData.Attack3.RangeX);
    loadedData.Attack3.RangeY = ini.GetShort("Attack3", "RangeY", loadedData.Attack3.RangeY);
    loadedData.Attack3.Damage = ini.GetByte("Attack3", "Damage", loadedData.Attack3.Damage);
    loadedData.Attack3.CooldownMs = ini.GetUInt32("Attack3", "CooldownMs", loadedData.Attack3.CooldownMs);

    loadedData.ServerDebug.RenderPlayerLineMax = ini.GetInt("ServerDebug", "RenderPlayerLineMax", loadedData.ServerDebug.RenderPlayerLineMax);
    loadedData.ServerDebug.RenderClearExtraLineCount = ini.GetInt("ServerDebug", "RenderClearExtraLineCount", loadedData.ServerDebug.RenderClearExtraLineCount);
    loadedData.ServerDebug.MaxFixedDeltaMs = ini.GetUInt32("ServerDebug", "MaxFixedDeltaMs", loadedData.ServerDebug.MaxFixedDeltaMs);

    NormalizeGameData(loadedData);
    outData = loadedData;
    return true;
}
