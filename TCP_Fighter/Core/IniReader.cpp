#include "IniReader.h"

#include <algorithm>
#include <cctype>
#include <fstream>

void CIniReader::Clear()
{
    m_Data.clear();
}

bool CIniReader::Load(const char* filePath)
{
    Clear();

    if (filePath == nullptr)
        return false;

    std::ifstream file(filePath);
    if (!file.is_open())
        return false;

    std::string currentSection;
    std::string line;

    while (std::getline(file, line))
    {
        const std::size_t commentPos = line.find_first_of(";#");
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

        const std::size_t equalPos = line.find('=');
        if (equalPos == std::string::npos)
            continue;

        std::string key = Trim(line.substr(0, equalPos));
        std::string value = Trim(line.substr(equalPos + 1));

        if (!currentSection.empty() && !key.empty())
            m_Data[currentSection][key] = value;
    }

    return true;
}

std::string CIniReader::GetString(const char* section, const char* key, const std::string& defaultValue) const
{
    if (section == nullptr || key == nullptr)
        return defaultValue;

    const auto sectionIt = m_Data.find(section);
    if (sectionIt == m_Data.end())
        return defaultValue;

    const auto keyIt = sectionIt->second.find(key);
    if (keyIt == sectionIt->second.end())
        return defaultValue;

    return keyIt->second;
}

int CIniReader::GetInt(const char* section, const char* key, int defaultValue) const
{
    const std::string value = GetString(section, key, std::string());
    if (value.empty())
        return defaultValue;

    try
    {
        return std::stoi(value);
    }
    catch (...)
    {
        return defaultValue;
    }
}

short CIniReader::GetShort(const char* section, const char* key, short defaultValue) const
{
    int value = GetInt(section, key, static_cast<int>(defaultValue));
    value = ClampInt(value, -32768, 32767);
    return static_cast<short>(value);
}

std::uint8_t CIniReader::GetByte(const char* section, const char* key, std::uint8_t defaultValue) const
{
    int value = GetInt(section, key, static_cast<int>(defaultValue));
    value = ClampInt(value, 0, 255);
    return static_cast<std::uint8_t>(value);
}

std::uint32_t CIniReader::GetUInt32(const char* section, const char* key, std::uint32_t defaultValue) const
{
    const std::string value = GetString(section, key, std::string());
    if (value.empty())
        return defaultValue;

    try
    {
        const unsigned long parsed = std::stoul(value);
        if (parsed > 0xfffffffful)
            return 0xffffffffu;

        return static_cast<std::uint32_t>(parsed);
    }
    catch (...)
    {
        return defaultValue;
    }
}

std::uint64_t CIniReader::GetUInt64(const char* section, const char* key, std::uint64_t defaultValue) const
{
    const std::string value = GetString(section, key, std::string());
    if (value.empty())
        return defaultValue;

    try
    {
        return static_cast<std::uint64_t>(std::stoull(value));
    }
    catch (...)
    {
        return defaultValue;
    }
}

bool CIniReader::GetBool(const char* section, const char* key, bool defaultValue) const
{
    std::string value = GetString(section, key, std::string());
    if (value.empty())
        return defaultValue;

    std::transform(value.begin(), value.end(), value.begin(),
        [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });

    if (value == "true" || value == "1" || value == "yes" || value == "on")
        return true;

    if (value == "false" || value == "0" || value == "no" || value == "off")
        return false;

    return defaultValue;
}

std::string CIniReader::Trim(const std::string& text)
{
    std::size_t begin = 0;
    while (begin < text.size() && std::isspace(static_cast<unsigned char>(text[begin])))
        ++begin;

    std::size_t end = text.size();
    while (end > begin && std::isspace(static_cast<unsigned char>(text[end - 1])))
        --end;

    return text.substr(begin, end - begin);
}

int CIniReader::ClampInt(int value, int minValue, int maxValue)
{
    if (value < minValue)
        return minValue;

    if (value > maxValue)
        return maxValue;

    return value;
}
