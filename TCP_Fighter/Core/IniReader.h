#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

class CIniReader
{
public:
    bool Load(const char* filePath);
    void Clear();

    std::string GetString(const char* section, const char* key, const std::string& defaultValue) const;
    int GetInt(const char* section, const char* key, int defaultValue) const;
    short GetShort(const char* section, const char* key, short defaultValue) const;
    std::uint8_t GetByte(const char* section, const char* key, std::uint8_t defaultValue) const;
    std::uint32_t GetUInt32(const char* section, const char* key, std::uint32_t defaultValue) const;
    std::uint64_t GetUInt64(const char* section, const char* key, std::uint64_t defaultValue) const;
    bool GetBool(const char* section, const char* key, bool defaultValue) const;

private:
    static std::string Trim(const std::string& text);
    static int ClampInt(int value, int minValue, int maxValue);

private:
    using KeyMap = std::unordered_map<std::string, std::string>;
    using SectionMap = std::unordered_map<std::string, KeyMap>;

    SectionMap m_Data;
};
