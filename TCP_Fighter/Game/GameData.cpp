#include "GameData.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

namespace
{
    std::string Trim(const std::string& text)
    {
        std::size_t begin = 0;
        while (begin < text.size() && std::isspace(static_cast<unsigned char>(text[begin])))
            ++begin;

        std::size_t end = text.size();
        while (end > begin && std::isspace(static_cast<unsigned char>(text[end - 1])))
            --end;

        return text.substr(begin, end - begin);
    }

    int ClampInt(int value, int minValue, int maxValue)
    {
        if (value < minValue)
            return minValue;

        if (value > maxValue)
            return maxValue;

        return value;
    }

    using SectionMap = std::unordered_map<std::string, std::unordered_map<std::string, std::string>>;

    int GetInt(const SectionMap& data, const char* section, const char* key, int defaultValue)
    {
        auto sectionIt = data.find(section);
        if (sectionIt == data.end())
            return defaultValue;

        auto keyIt = sectionIt->second.find(key);
        if (keyIt == sectionIt->second.end())
            return defaultValue;

        try
        {
            return std::stoi(keyIt->second);
        }
        catch (...)
        {
            return defaultValue;
        }
    }

    short GetShort(const SectionMap& data, const char* section, const char* key, short defaultValue)
    {
        int value = GetInt(data, section, key, static_cast<int>(defaultValue));
        value = ClampInt(value, -32768, 32767);
        return static_cast<short>(value);
    }

    std::uint8_t GetByte(const SectionMap& data, const char* section, const char* key, std::uint8_t defaultValue)
    {
        int value = GetInt(data, section, key, static_cast<int>(defaultValue));
        value = ClampInt(value, 0, 255);
        return static_cast<std::uint8_t>(value);
    }

    unsigned int GetUInt(const SectionMap& data, const char* section, const char* key, unsigned int defaultValue)
    {
        int value = GetInt(data, section, key, static_cast<int>(defaultValue));
        if (value < 0)
            value = 0;

        return static_cast<unsigned int>(value);
    }

    bool LoadIni(const char* filePath, SectionMap& outData)
    {
        if (filePath == nullptr)
            return false;

        std::ifstream file(filePath);
        if (!file.is_open())
            return false;

        std::string currentSection;
        std::string line;

        while (std::getline(file, line))
        {
            std::size_t commentPos = line.find_first_of(";#");
            if (commentPos != std::string::npos)
                line = line.substr(0, commentPos);

            line = Trim(line);
            if (line.empty())
                continue;

            if (line.front() == '[' && line.back() == ']')
            {
                currentSection = Trim(line.substr(1, line.length() - 2));
                continue;
            }

            std::size_t equalPos = line.find('=');
            if (equalPos == std::string::npos)
                continue;

            std::string key = Trim(line.substr(0, equalPos));
            std::string value = Trim(line.substr(equalPos + 1));

            if (!currentSection.empty() && !key.empty())
                outData[currentSection][key] = value;
        }

        return true;
    }

    void NormalizeAttackGameData(AttackGameData& attackData)
    {
        if (attackData.RangeX < 0)
            attackData.RangeX = 0;

        if (attackData.RangeY < 0)
            attackData.RangeY = 0;

        if (attackData.CooldownMs == 0)
            attackData.CooldownMs = 1;
    }

    void NormalizeGameData(GameData& data)
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
    SectionMap ini;
    if (!LoadIni(filePath, ini))
        return false;

    GameData loadedData = outData;

    loadedData.World.ScreenWidth = GetShort(ini, "World", "ScreenWidth", loadedData.World.ScreenWidth);
    loadedData.World.ScreenHeight = GetShort(ini, "World", "ScreenHeight", loadedData.World.ScreenHeight);
    loadedData.World.MoveTop = GetShort(ini, "World", "MoveTop", loadedData.World.MoveTop);
    loadedData.World.MoveLeft = GetShort(ini, "World", "MoveLeft", loadedData.World.MoveLeft);
    loadedData.World.MoveRight = GetShort(ini, "World", "MoveRight", loadedData.World.MoveRight);
    loadedData.World.MoveBottom = GetShort(ini, "World", "MoveBottom", loadedData.World.MoveBottom);

    loadedData.Move.MoveXPerFrame = GetShort(ini, "Move", "MoveXPerFrame", loadedData.Move.MoveXPerFrame);
    loadedData.Move.MoveYPerFrame = GetShort(ini, "Move", "MoveYPerFrame", loadedData.Move.MoveYPerFrame);
    loadedData.Move.ErrorRange = GetShort(ini, "Move", "ErrorRange", loadedData.Move.ErrorRange);

    loadedData.Character.DefaultHP = GetByte(ini, "Character", "DefaultHP", loadedData.Character.DefaultHP);

    loadedData.Attack.InputAdvanceMs = GetUInt(ini, "Attack", "InputAdvanceMs", loadedData.Attack.InputAdvanceMs);

    loadedData.Attack1.RangeX = GetShort(ini, "Attack1", "RangeX", loadedData.Attack1.RangeX);
    loadedData.Attack1.RangeY = GetShort(ini, "Attack1", "RangeY", loadedData.Attack1.RangeY);
    loadedData.Attack1.Damage = GetByte(ini, "Attack1", "Damage", loadedData.Attack1.Damage);
    loadedData.Attack1.CooldownMs = GetUInt(ini, "Attack1", "CooldownMs", loadedData.Attack1.CooldownMs);

    loadedData.Attack2.RangeX = GetShort(ini, "Attack2", "RangeX", loadedData.Attack2.RangeX);
    loadedData.Attack2.RangeY = GetShort(ini, "Attack2", "RangeY", loadedData.Attack2.RangeY);
    loadedData.Attack2.Damage = GetByte(ini, "Attack2", "Damage", loadedData.Attack2.Damage);
    loadedData.Attack2.CooldownMs = GetUInt(ini, "Attack2", "CooldownMs", loadedData.Attack2.CooldownMs);

    loadedData.Attack3.RangeX = GetShort(ini, "Attack3", "RangeX", loadedData.Attack3.RangeX);
    loadedData.Attack3.RangeY = GetShort(ini, "Attack3", "RangeY", loadedData.Attack3.RangeY);
    loadedData.Attack3.Damage = GetByte(ini, "Attack3", "Damage", loadedData.Attack3.Damage);
    loadedData.Attack3.CooldownMs = GetUInt(ini, "Attack3", "CooldownMs", loadedData.Attack3.CooldownMs);

    loadedData.ServerDebug.RenderPlayerLineMax = GetInt(
        ini,
        "ServerDebug",
        "RenderPlayerLineMax",
        loadedData.ServerDebug.RenderPlayerLineMax
    );

    loadedData.ServerDebug.RenderClearExtraLineCount = GetInt(
        ini,
        "ServerDebug",
        "RenderClearExtraLineCount",
        loadedData.ServerDebug.RenderClearExtraLineCount
    );

    loadedData.ServerDebug.MaxFixedDeltaMs = GetUInt(
        ini,
        "ServerDebug",
        "MaxFixedDeltaMs",
        loadedData.ServerDebug.MaxFixedDeltaMs
    );

    NormalizeGameData(loadedData);
    outData = loadedData;
    return true;
}