#pragma once

#include <cstdint>

struct AttackGameData
{
    short RangeX = 0;
    short RangeY = 0;
    std::uint8_t Damage = 0;
    unsigned int CastTick = 0;
};

struct WorldGameData
{
    short ScreenWidth = 640;
    short ScreenHeight = 480;

    short MoveTop = 50;
    short MoveLeft = 10;
    short MoveRight = 630;
    short MoveBottom = 470;
};

struct MoveGameData
{
    short MoveXPerTick = 3;
    short MoveYPerTick = 2;
    short ErrorRange = 50;
};

struct CharacterGameData
{
    std::uint8_t DefaultHP = 100;
};

struct ServerDebugGameData
{
    int RenderPlayerLineMax = 10;
    int RenderClearExtraLineCount = 5;
    unsigned __int64 MaxFixedDeltaMs = 200;
};

struct GameData
{
    WorldGameData World;
    MoveGameData Move;
    CharacterGameData Character;
    AttackGameData Attack1;
    AttackGameData Attack2;
    AttackGameData Attack3;
    ServerDebugGameData ServerDebug;
};

class CGameDataLoader
{
public:
    static bool LoadGameData(const char* filePath, GameData& outData);
};
