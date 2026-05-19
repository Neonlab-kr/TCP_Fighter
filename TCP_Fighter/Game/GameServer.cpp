#include "GameServer.h"
#include "GameDefine.h"
#include "GameData.h"
#include "PacketMaker.h"
#include "../Core/Config.h"
#include "../Core/Logger.h"
#include "../Core/Session.h"

#include <Windows.h>
#include <mmsystem.h>
#include <cstdlib>

CGameServer::CGameServer()
{
    m_Running = false;
    m_NextPlayerId = 1;
    m_MaxGameSession = 0;
    m_ActiveGameSessionCount = 0;
    m_CurrentTimeMs = 0;
    m_LastLoopTimeMs = 0;
    m_FixedUpdateAccumulatorMs = 0;
    m_ClientFrameMs = 20;
}

CGameServer::~CGameServer()
{
    Release();
}

bool CGameServer::Init(const char* configPath)
{
    if (!CConfigLoader::LoadServerConfig(configPath, m_Config))
        CLogger::Error("Config load failed. use default config: %s", configPath);

    if (!CGameDataLoader::LoadGameData(dfDEFAULT_GAME_DATA_PATH, m_GameData))
        CLogger::Error("GameData load failed. use default game data: %s", dfDEFAULT_GAME_DATA_PATH);

    if (!CSelectServer::Init(m_Config))
        return false;

    m_MaxGameSession = GetMaxSession();
    m_GameSessions.reset(new GameSession[m_MaxGameSession]);
    m_DeadSessionBuffer.reset(new CSession*[m_MaxGameSession]);

    m_ActiveGameSessionCount = 0;
    m_ClientFrameMs = GetClientFrameMs();
    m_CurrentTimeMs = timeGetTime();
    m_LastLoopTimeMs = m_CurrentTimeMs;
    m_FixedUpdateAccumulatorMs = 0;
    m_Running = true;
    return true;
}

void CGameServer::Run()
{
    while (m_Running)
    {
        m_CurrentTimeMs = timeGetTime();
        std::uint32_t delta = m_CurrentTimeMs - m_LastLoopTimeMs;
        m_LastLoopTimeMs = m_CurrentTimeMs;

        if (delta > m_GameData.ServerDebug.MaxFixedDeltaMs)
            delta = m_GameData.ServerDebug.MaxFixedDeltaMs;

        m_FixedUpdateAccumulatorMs += delta;

        UpdateNetwork();

        while (m_FixedUpdateAccumulatorMs >= m_ClientFrameMs)
        {
            FixedUpdateGame();
            m_FixedUpdateAccumulatorMs -= m_ClientFrameMs;
        }

        Render();
        Sleep(0);
    }
}

void CGameServer::Stop()
{
    m_Running = false;
}

void CGameServer::OnAccept(CSession* session)
{
    if (session == nullptr)
        return;

    const int slotIndex = session->GetSlotIndex();
    if (slotIndex < 0 || slotIndex >= m_MaxGameSession)
    {
        Disconnect(session);
        return;
    }

    GameSession& joined = m_GameSessions[slotIndex];
    joined.NetSession = session;
    joined.AttackLockUntilTimeMs = 0;
    joined.PlayerId = m_NextPlayerId++;
    joined.X = ClampX(static_cast<short>(100 + (std::rand() % 300)));
    joined.Y = ClampY(static_cast<short>(100 + (std::rand() % 200)));
    joined.Direction = dfPACKET_MOVE_DIR_RR;
    joined.HP = m_GameData.Character.DefaultHP;
    joined.Moving = false;
    joined.Active = true;

    session->UserData = &joined;
    ++m_ActiveGameSessionCount;

    CLogger::Info("Client accepted. session=%llu, player=%d, total=%d", session->GetSessionId(), joined.PlayerId, GetSessionCount());

    st_PACKET_SC_CREATE_MY_CHARACTER myPacket{};
    MakePacket_CreateMyCharacter(myPacket, joined.PlayerId, joined.Direction, joined.X, joined.Y, joined.HP);
    SendUnicast(session, myPacket);

    st_PACKET_SC_CREATE_OTHER_CHARACTER newOtherPacket{};
    MakePacket_CreateOtherCharacter(newOtherPacket, joined.PlayerId, joined.Direction, joined.X, joined.Y, joined.HP);
    SendBroadcast(session, newOtherPacket);

    for (int i = 0; i < m_MaxGameSession; ++i)
    {
        const GameSession& other = m_GameSessions[i];

        if (!other.Active || other.NetSession == session)
            continue;

        st_PACKET_SC_CREATE_OTHER_CHARACTER oldOtherPacket{};
        MakePacket_CreateOtherCharacter(oldOtherPacket, other.PlayerId, other.Direction, other.X, other.Y, other.HP);
        SendUnicast(session, oldOtherPacket);
    }
}

void CGameServer::OnRelease(CSession* session)
{
    GameSession* released = FindGameSession(session);

    if (released == nullptr || !released->Active)
        return;

    const int playerId = released->PlayerId;

    CLogger::Info("Client released. session=%llu, player=%d", session->GetSessionId(), playerId);

    st_PACKET_SC_DELETE_CHARACTER deletePacket{};
    MakePacket_DeleteCharacter(deletePacket, playerId);
    SendBroadcast(session, deletePacket);

    released->Active = false;
    released->NetSession = nullptr;
    released->AttackLockUntilTimeMs = 0;
    released->PlayerId = -1;
    released->X = 0;
    released->Y = 0;
    released->Direction = 0;
    released->HP = 0;
    released->Moving = false;
    session->UserData = nullptr;

    if (m_ActiveGameSessionCount > 0)
        --m_ActiveGameSessionCount;
}

void CGameServer::OnError(int errorCode)
{
    CLogger::Error("Network error: %d", errorCode);
}
