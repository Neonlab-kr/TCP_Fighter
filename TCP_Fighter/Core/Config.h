#pragma once

#include <string>

struct ServerConfig
{
    std::string BindIP = "0.0.0.0";
    unsigned short Port = 5000;
    int ClientFPS = 50;
    int MaxSession = 63;
    int RecvBufferSize = 8192;
    int SendBufferSize = 8192;
    bool TcpNoDelay = true;
    bool EnableRender = true;
};

class CConfigLoader
{
public:
    static bool LoadServerConfig(const std::string& path, ServerConfig& outConfig);
};
