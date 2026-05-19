#include "Config.h"
#include "IniReader.h"

bool CConfigLoader::LoadServerConfig(const std::string& path, ServerConfig& outConfig)
{
    CIniReader ini;
    if (!ini.Load(path.c_str()))
        return false;

    outConfig.BindIP = ini.GetString("Server", "BindIP", outConfig.BindIP);
    outConfig.Port = static_cast<unsigned short>(ini.GetInt("Server", "Port", outConfig.Port));
    outConfig.ClientFPS = ini.GetInt("Server", "ClientFPS", outConfig.ClientFPS);
    outConfig.MaxSession = ini.GetInt("Server", "MaxSession", outConfig.MaxSession);
    outConfig.RecvBufferSize = ini.GetInt("Server", "RecvBufferSize", outConfig.RecvBufferSize);
    outConfig.SendBufferSize = ini.GetInt("Server", "SendBufferSize", outConfig.SendBufferSize);
    outConfig.TcpNoDelay = ini.GetBool("Server", "TcpNoDelay", outConfig.TcpNoDelay);
    outConfig.EnableRender = ini.GetBool("Server", "EnableRender", outConfig.EnableRender);

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
