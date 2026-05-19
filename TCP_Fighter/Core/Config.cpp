#include "Config.h"
#include "FileReader.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <sstream>

bool CConfigLoader::LoadServerConfig(const std::string& path, ServerConfig& outConfig)
{
    std::string text;

    if (!CFileReader::ReadText(path, text))
        return false;

    std::istringstream stream(text);
    std::string line;

    while (std::getline(stream, line))
    {
        line = Trim(line);

        if (line.empty())
            continue;

        if (line[0] == '#' || line[0] == ';')
            continue;

        if (line.front() == '[' && line.back() == ']')
            continue;

        std::size_t equalPos = line.find('=');

        if (equalPos == std::string::npos)
            continue;

        std::string key = Trim(line.substr(0, equalPos));
        std::string value = Trim(line.substr(equalPos + 1));

        if (key == "BindIP")
            outConfig.BindIP = value;
        else if (key == "Port")
            outConfig.Port = static_cast<unsigned short>(std::atoi(value.c_str()));
        else if (key == "ClientFPS")
            outConfig.ClientFPS = std::atoi(value.c_str());
        else if (key == "MaxSession")
            outConfig.MaxSession = std::atoi(value.c_str());
        else if (key == "RecvBufferSize")
            outConfig.RecvBufferSize = std::atoi(value.c_str());
        else if (key == "SendBufferSize")
            outConfig.SendBufferSize = std::atoi(value.c_str());
        else if (key == "TcpNoDelay")
            outConfig.TcpNoDelay = ToBool(value, outConfig.TcpNoDelay);
    }

    if (outConfig.Port == 0)
        outConfig.Port = 5000;

    if (outConfig.ClientFPS <= 0)
        outConfig.ClientFPS = 50;

    if (outConfig.MaxSession <= 0)
        outConfig.MaxSession = 63;

    if (outConfig.MaxSession > 63)
        outConfig.MaxSession = 63;

    if (outConfig.RecvBufferSize <= 0)
        outConfig.RecvBufferSize = 8192;

    if (outConfig.SendBufferSize <= 0)
        outConfig.SendBufferSize = 8192;

    return true;
}

std::string CConfigLoader::Trim(const std::string& text)
{
    std::size_t begin = 0;
    std::size_t end = text.size();

    while (begin < end && std::isspace(static_cast<unsigned char>(text[begin])))
        ++begin;

    while (end > begin && std::isspace(static_cast<unsigned char>(text[end - 1])))
        --end;

    return text.substr(begin, end - begin);
}

bool CConfigLoader::ToBool(const std::string& text, bool defaultValue)
{
    std::string lower = text;

    std::transform(lower.begin(), lower.end(), lower.begin(),
        [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });

    if (lower == "true" || lower == "1" || lower == "yes" || lower == "on")
        return true;

    if (lower == "false" || lower == "0" || lower == "no" || lower == "off")
        return false;

    return defaultValue;
}
