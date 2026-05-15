#include "GameServer.h"
#include "PacketDefine.h"
#include "GameDefine.h"
#include "GameData.h"
#include "../Core/Config.h"
#include "../Core/Logger.h"
#include "../Core/Session.h"

#include <Windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

static inline void PrintClearLine(HANDLE console, int consoleWidth, const char* text)
{
    if (text == nullptr)
        text = "";

    DWORD written = 0;
    const int length = static_cast<int>(std::strlen(text));
    WriteConsoleA(console, text, static_cast<DWORD>(length), &written, nullptr);

    int padding = consoleWidth - length;
    static const char spaces[] = "                                                                                                                                ";

    while (padding > 0)
    {
        const int chunk = padding > static_cast<int>(sizeof(spaces) - 1) ? static_cast<int>(sizeof(spaces) - 1) : padding;
        WriteConsoleA(console, spaces, static_cast<DWORD>(chunk), &written, nullptr);
        padding -= chunk;
    }

    WriteConsoleA(console, "\n", 1, &written, nullptr);
}

CGameServer::CGameServer()
{
    m_Running = false;
    m_NextPlayerId = 1;
    m_MaxGameSession = 0;
    m_ActiveGameSessionCount = 0;
    m_LastLoopTickMs = 0;
    m_FixedUpdateAccumulatorMs = 0;
    m_FixedTick = 0;
    m_ClientTickMs = 20;
}

CGameServer::~CGameServer()
{
    Release();
}

bool CGameServer::Init(const char* configPath)
{
    if (!CConfigLoader::LoadServerConfig(configPath, m_Config))
    {
        CLogger::Error("Config load failed. use default config: %s", configPath);
    }

    if (!CGameDataLoader::LoadGameData(dfDEFAULT_GAME_DATA_PATH, m_GameData))
    {
        CLogger::Error("GameData load failed. use default game data: %s", dfDEFAULT_GAME_DATA_PATH);
    }

    if (!CAsyncSelectServer::Init(m_Config))
    {
        return false;
    }

    m_MaxGameSession = GetMaxSession();

    m_GameSessions.reset(new GameSession[m_MaxGameSession]);
    m_DeadSessionBuffer.reset(new CSession*[m_MaxGameSession]);

    m_ActiveGameSessionCount = 0;

    m_ClientTickMs = GetClientTickMs();
    m_LastLoopTickMs = GetTickCount64();
    m_FixedUpdateAccumulatorMs = 0;
    m_FixedTick = 0;

    m_Running = true;

    return true;
}

void CGameServer::Run()
{
    while (m_Running)
    {
        const unsigned __int64 now = GetTickCount64();
        unsigned __int64 delta = now - m_LastLoopTickMs;
        m_LastLoopTickMs = now;

        if (delta > m_GameData.ServerDebug.MaxFixedDeltaMs)
            delta = m_GameData.ServerDebug.MaxFixedDeltaMs;

        m_FixedUpdateAccumulatorMs += delta;

        UpdateNetwork();

        while (m_FixedUpdateAccumulatorMs >= m_ClientTickMs)
        {
            ++m_FixedTick;
            FixedUpdateGame();
            m_FixedUpdateAccumulatorMs -= m_ClientTickMs;
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
    joined.AttackLockUntilTick = 0;
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
    myPacket.header = MakePacketHeader<st_PACKET_SC_CREATE_MY_CHARACTER>(dfPACKET_SC_CREATE_MY_CHARACTER);
    myPacket.id = joined.PlayerId;
    myPacket.direction = joined.Direction;
    myPacket.x = joined.X;
    myPacket.y = joined.Y;
    myPacket.hp = joined.HP;
    SendUnicast(session, myPacket);

    st_PACKET_SC_CREATE_OTHER_CHARACTER newOtherPacket{};
    newOtherPacket.header = MakePacketHeader<st_PACKET_SC_CREATE_OTHER_CHARACTER>(dfPACKET_SC_CREATE_OTHER_CHARACTER);
    newOtherPacket.id = joined.PlayerId;
    newOtherPacket.direction = joined.Direction;
    newOtherPacket.x = joined.X;
    newOtherPacket.y = joined.Y;
    newOtherPacket.hp = joined.HP;
    SendBroadcast(session, newOtherPacket);

    for (int i = 0; i < m_MaxGameSession; ++i)
    {
        const GameSession& other = m_GameSessions[i];

        if (!other.Active || other.NetSession == session)
            continue;

        st_PACKET_SC_CREATE_OTHER_CHARACTER oldOtherPacket{};
        oldOtherPacket.header = MakePacketHeader<st_PACKET_SC_CREATE_OTHER_CHARACTER>(dfPACKET_SC_CREATE_OTHER_CHARACTER);
        oldOtherPacket.id = other.PlayerId;
        oldOtherPacket.direction = other.Direction;
        oldOtherPacket.x = other.X;
        oldOtherPacket.y = other.Y;
        oldOtherPacket.hp = other.HP;
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
    deletePacket.header = MakePacketHeader<st_PACKET_SC_DELETE_CHARACTER>(dfPACKET_SC_DELETE_CHARACTER);
    deletePacket.id = playerId;
    SendBroadcast(session, deletePacket);

    released->Active = false;
    released->NetSession = nullptr;
    released->AttackLockUntilTick = 0;
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

void CGameServer::OnRecv(CSession* session)
{
    if (session == nullptr)
        return;

    while (true)
    {
        if (session->RecvBuffer.GetUseSize() < static_cast<int>(sizeof(st_PACKET_HEADER)))
            return;

        st_PACKET_HEADER header{};
        const int peekRet = session->RecvBuffer.Peek(reinterpret_cast<char*>(&header), sizeof(header));

        if (peekRet != sizeof(header))
            return;

        if (header.byCode != dfPACKET_CODE)
        {
            CLogger::Error("Invalid packet code. session=%llu, code=0x%02x", session->GetSessionId(), header.byCode);
            Disconnect(session);
            return;
        }

        const int packetSize = static_cast<int>(sizeof(st_PACKET_HEADER)) + header.bySize;

        if (packetSize <= static_cast<int>(sizeof(st_PACKET_HEADER)) || packetSize > dfPACKET_MAX_SIZE)
        {
            CLogger::Error("Invalid packet size. session=%llu, size=%d", session->GetSessionId(), packetSize);
            Disconnect(session);
            return;
        }

        if (session->RecvBuffer.GetUseSize() < packetSize)
            return;

        char packet[dfPACKET_MAX_SIZE]{};
        const int dequeueRet = session->RecvBuffer.Dequeue(packet, packetSize);

        if (dequeueRet != packetSize)
            return;

        ProcessPacket(session, packet, packetSize);

        if (session->DisconnectPending)
            return;
    }
}

void CGameServer::OnError(int errorCode)
{
    CLogger::Error("Network error: %d", errorCode);
}

void CGameServer::FixedUpdateGame()
{
    for (int i = 0; i < m_MaxGameSession; ++i)
    {
        GameSession& gameSession = m_GameSessions[i];

        if (!gameSession.Active)
            continue;

        CSession* session = gameSession.NetSession;
        if (session == nullptr || session->DisconnectPending)
            continue;

        if (gameSession.Moving)
            UpdateMove(gameSession);
    }
}

void CGameServer::Render()
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos{};
    pos.X = 0;
    pos.Y = 0;
    SetConsoleCursorPosition(console, pos);

    CONSOLE_SCREEN_BUFFER_INFO info{};
    int consoleWidth = 120;
    if (GetConsoleScreenBufferInfo(console, &info) && info.dwSize.X > 0)
        consoleWidth = info.dwSize.X;

    char lineBuffer[512];

    PrintClearLine(console, consoleWidth, "AsyncSelect TCP Game Server");

    std::snprintf(lineBuffer, sizeof(lineBuffer), "Bind        : %s:%hu", m_Config.BindIP.c_str(), m_Config.Port);
    PrintClearLine(console, consoleWidth, lineBuffer);

    std::snprintf(lineBuffer, sizeof(lineBuffer), "ClientFPS   : %d", m_Config.ClientFPS);
    PrintClearLine(console, consoleWidth, lineBuffer);

    std::snprintf(lineBuffer, sizeof(lineBuffer), "World       : %dx%d MoveRange=(%d,%d)-(%d,%d)",
        static_cast<int>(m_GameData.World.ScreenWidth),
        static_cast<int>(m_GameData.World.ScreenHeight),
        static_cast<int>(m_GameData.World.MoveLeft),
        static_cast<int>(m_GameData.World.MoveTop),
        static_cast<int>(m_GameData.World.MoveRight),
        static_cast<int>(m_GameData.World.MoveBottom));
    PrintClearLine(console, consoleWidth, lineBuffer);

    std::snprintf(lineBuffer, sizeof(lineBuffer), "Sessions    : %d", GetSessionCount());
    PrintClearLine(console, consoleWidth, lineBuffer);

    std::snprintf(lineBuffer, sizeof(lineBuffer), "Players     : %d", m_ActiveGameSessionCount);
    PrintClearLine(console, consoleWidth, lineBuffer);

    std::snprintf(lineBuffer, sizeof(lineBuffer), "FixedTick   : %llu", m_FixedTick);
    PrintClearLine(console, consoleWidth, lineBuffer);

    int line = 0;

    for (int i = 0; i < m_MaxGameSession && line < m_GameData.ServerDebug.RenderPlayerLineMax; ++i)
    {
        const GameSession& p = m_GameSessions[i];

        if (!p.Active)
            continue;

        std::snprintf(lineBuffer, sizeof(lineBuffer), "Player[%d] ID=%d X=%d Y=%d Dir=%d HP=%d Moving=%s AttackLock=%s",
            line,
            p.PlayerId,
            static_cast<int>(p.X),
            static_cast<int>(p.Y),
            static_cast<int>(p.Direction),
            static_cast<int>(p.HP),
            p.Moving ? "Y" : "N",
            (m_FixedTick < p.AttackLockUntilTick) ? "Y" : "N");

        PrintClearLine(console, consoleWidth, lineBuffer);
        ++line;
    }

    for (; line < m_GameData.ServerDebug.RenderPlayerLineMax + m_GameData.ServerDebug.RenderClearExtraLineCount; ++line)
        PrintClearLine(console, consoleWidth, "");
}

void CGameServer::ProcessPacket(CSession* session, const char* packet, int packetSize)
{
    if (session == nullptr || packet == nullptr || packetSize <= 0)
        return;

    GameSession* gameSession = FindGameSession(session);

    if (gameSession == nullptr || !gameSession->Active)
    {
        Disconnect(session);
        return;
    }

    const st_PACKET_HEADER* header = reinterpret_cast<const st_PACKET_HEADER*>(packet);

    switch (header->byType)
    {
    case dfPACKET_CS_MOVE_START:
    {
        if (!ValidatePacketSize(packet, packetSize, sizeof(st_PACKET_CS_MOVE_START)))
        {
            Disconnect(session);
            return;
        }

        const st_PACKET_CS_MOVE_START* recvPacket = reinterpret_cast<const st_PACKET_CS_MOVE_START*>(packet);

        if (!ValidateDirection(recvPacket->direction))
        {
            Disconnect(session);
            return;
        }

        if (IsAttackLocked(*gameSession))
        {
            SendSync(*gameSession, session);
            return;
        }

        if (!IsClientPositionInErrorRange(*gameSession, recvPacket->x, recvPacket->y))
        {
            SendSync(*gameSession, session);
            return;
        }

        gameSession->Direction = recvPacket->direction;
        gameSession->Moving = true;

        st_PACKET_SC_MOVE_START sendPacket{};
        sendPacket.header = MakePacketHeader<st_PACKET_SC_MOVE_START>(dfPACKET_SC_MOVE_START);
        sendPacket.id = gameSession->PlayerId;
        sendPacket.direction = gameSession->Direction;
        sendPacket.x = gameSession->X;
        sendPacket.y = gameSession->Y;
        SendBroadcast(session, sendPacket);
        break;
    }

    case dfPACKET_CS_MOVE_STOP:
    {
        if (!ValidatePacketSize(packet, packetSize, sizeof(st_PACKET_CS_MOVE_STOP)))
        {
            Disconnect(session);
            return;
        }

        const st_PACKET_CS_MOVE_STOP* recvPacket = reinterpret_cast<const st_PACKET_CS_MOVE_STOP*>(packet);

        if (!ValidateDirection(recvPacket->direction))
        {
            Disconnect(session);
            return;
        }

        if (!IsClientPositionInErrorRange(*gameSession, recvPacket->x, recvPacket->y))
        {
            SendSync(*gameSession, session);
            return;
        }

        gameSession->Direction = recvPacket->direction;
        gameSession->Moving = false;

        st_PACKET_SC_MOVE_STOP sendPacket{};
        sendPacket.header = MakePacketHeader<st_PACKET_SC_MOVE_STOP>(dfPACKET_SC_MOVE_STOP);
        sendPacket.id = gameSession->PlayerId;
        sendPacket.direction = gameSession->Direction;
        sendPacket.x = gameSession->X;
        sendPacket.y = gameSession->Y;
        SendBroadcast(session, sendPacket);
        break;
    }

    case dfPACKET_CS_ATTACK1:
    case dfPACKET_CS_ATTACK2:
    case dfPACKET_CS_ATTACK3:
    {
        if (!ValidatePacketSize(packet, packetSize, sizeof(st_PACKET_CS_ATTACK1)))
        {
            Disconnect(session);
            return;
        }

        const st_PACKET_CS_ATTACK1* recvPacket = reinterpret_cast<const st_PACKET_CS_ATTACK1*>(packet);

        if (!ValidateDirection(recvPacket->direction))
        {
            Disconnect(session);
            return;
        }

        if (IsAttackLocked(*gameSession))
        {
            SendSync(*gameSession, session);
            return;
        }

        if (!IsClientPositionInErrorRange(*gameSession, recvPacket->x, recvPacket->y))
        {
            SendSync(*gameSession, session);
            return;
        }

        gameSession->Direction = recvPacket->direction;
        gameSession->Moving = false;

        if (header->byType == dfPACKET_CS_ATTACK1)
        {
            st_PACKET_SC_ATTACK1 sendPacket{};
            sendPacket.header = MakePacketHeader<st_PACKET_SC_ATTACK1>(dfPACKET_SC_ATTACK1);
            sendPacket.id = gameSession->PlayerId;
            sendPacket.direction = gameSession->Direction;
            sendPacket.x = gameSession->X;
            sendPacket.y = gameSession->Y;
            SendBroadcast(session, sendPacket);
        }
        else if (header->byType == dfPACKET_CS_ATTACK2)
        {
            st_PACKET_SC_ATTACK2 sendPacket{};
            sendPacket.header = MakePacketHeader<st_PACKET_SC_ATTACK2>(dfPACKET_SC_ATTACK2);
            sendPacket.id = gameSession->PlayerId;
            sendPacket.direction = gameSession->Direction;
            sendPacket.x = gameSession->X;
            sendPacket.y = gameSession->Y;
            SendBroadcast(session, sendPacket);
        }
        else
        {
            st_PACKET_SC_ATTACK3 sendPacket{};
            sendPacket.header = MakePacketHeader<st_PACKET_SC_ATTACK3>(dfPACKET_SC_ATTACK3);
            sendPacket.id = gameSession->PlayerId;
            sendPacket.direction = gameSession->Direction;
            sendPacket.x = gameSession->X;
            sendPacket.y = gameSession->Y;
            SendBroadcast(session, sendPacket);
        }

        ProcessAttack(*gameSession, header->byType);
        break;
    }

    case dfPACKET_CS_SYNC:
    {
        if (!ValidatePacketSize(packet, packetSize, sizeof(st_PACKET_CS_SYNC)))
        {
            Disconnect(session);
            return;
        }

        const st_PACKET_CS_SYNC* recvPacket = reinterpret_cast<const st_PACKET_CS_SYNC*>(packet);

        if (!IsClientPositionInErrorRange(*gameSession, recvPacket->x, recvPacket->y))
            SendSync(*gameSession, session);

        break;
    }

    default:
        CLogger::Error("Unknown packet type. session=%llu, type=%u", session->GetSessionId(), header->byType);
        Disconnect(session);
        break;
    }
}

void CGameServer::UpdateMove(GameSession& gameSession)
{
    if (IsAttackLocked(gameSession))
    {
        StopMove(gameSession, true);
        return;
    }

    short dx = 0;
    short dy = 0;

    switch (gameSession.Direction)
    {
    case dfPACKET_MOVE_DIR_LL:
        dx = -m_GameData.Move.MoveXPerTick;
        break;

    case dfPACKET_MOVE_DIR_LU:
        dx = -m_GameData.Move.MoveXPerTick;
        dy = -m_GameData.Move.MoveYPerTick;
        break;

    case dfPACKET_MOVE_DIR_UU:
        dy = -m_GameData.Move.MoveYPerTick;
        break;

    case dfPACKET_MOVE_DIR_RU:
        dx = m_GameData.Move.MoveXPerTick;
        dy = -m_GameData.Move.MoveYPerTick;
        break;

    case dfPACKET_MOVE_DIR_RR:
        dx = m_GameData.Move.MoveXPerTick;
        break;

    case dfPACKET_MOVE_DIR_RD:
        dx = m_GameData.Move.MoveXPerTick;
        dy = m_GameData.Move.MoveYPerTick;
        break;

    case dfPACKET_MOVE_DIR_DD:
        dy = m_GameData.Move.MoveYPerTick;
        break;

    case dfPACKET_MOVE_DIR_LD:
        dx = -m_GameData.Move.MoveXPerTick;
        dy = m_GameData.Move.MoveYPerTick;
        break;

    default:
        StopMove(gameSession, true);
        return;
    }

    const short nextX = static_cast<short>(gameSession.X + dx);
    const short nextY = static_cast<short>(gameSession.Y + dy);

    if (nextX <= m_GameData.World.MoveLeft || nextX >= m_GameData.World.MoveRight || nextY <= m_GameData.World.MoveTop || nextY >= m_GameData.World.MoveBottom)
    {
        StopMove(gameSession, true);
        return;
    }

    gameSession.X = nextX;
    gameSession.Y = nextY;
}

void CGameServer::StopMove(GameSession& gameSession, bool sendPacket)
{
    if (!gameSession.Moving && !sendPacket)
        return;

    gameSession.Moving = false;

    if (sendPacket)
        SendMoveStop(gameSession);
}

void CGameServer::SendMoveStop(const GameSession& gameSession)
{
    if (gameSession.NetSession == nullptr)
        return;

    st_PACKET_SC_MOVE_STOP packet{};
    packet.header = MakePacketHeader<st_PACKET_SC_MOVE_STOP>(dfPACKET_SC_MOVE_STOP);
    packet.id = gameSession.PlayerId;
    packet.direction = gameSession.Direction;
    packet.x = gameSession.X;
    packet.y = gameSession.Y;
    SendBroadcast(nullptr, packet);
}

void CGameServer::SendSync(const GameSession& gameSession, CSession* targetSession)
{
    if (targetSession == nullptr)
        return;

    st_PACKET_SC_SYNC packet{};
    packet.header = MakePacketHeader<st_PACKET_SC_SYNC>(dfPACKET_SC_SYNC);
    packet.id = gameSession.PlayerId;
    packet.x = gameSession.X;
    packet.y = gameSession.Y;
    SendUnicast(targetSession, packet);
}

void CGameServer::ProcessAttack(GameSession& attacker, std::uint8_t attackType)
{
    const AttackGameData* attackData = GetAttackData(attackType);

    if (attackData == nullptr || attackData->RangeX <= 0 || attackData->RangeY <= 0 || attackData->Damage == 0 || attackData->CastTick == 0)
        return;

    attacker.AttackLockUntilTick = m_FixedTick + attackData->CastTick;

    int deadCount = 0;

    for (int i = 0; i < m_MaxGameSession; ++i)
    {
        GameSession& target = m_GameSessions[i];

        if (!target.Active)
            continue;

        CSession* targetSession = target.NetSession;
        if (targetSession == nullptr || targetSession->DisconnectPending)
            continue;

        if (target.PlayerId == attacker.PlayerId || target.HP == 0)
            continue;

        if (!IsHitByAttack(attacker, target, attackData->RangeX, attackData->RangeY))
            continue;

        if (target.HP <= attackData->Damage)
            target.HP = 0;
        else
            target.HP = static_cast<std::uint8_t>(target.HP - attackData->Damage);

        SendDamage(attacker, target);

        if (target.HP == 0 && deadCount < m_MaxGameSession)
            m_DeadSessionBuffer[deadCount++] = targetSession;
    }

    for (int i = 0; i < deadCount; ++i)
        ProcessDeadPlayer(m_DeadSessionBuffer[i]);
}

bool CGameServer::IsHitByAttack(const GameSession& attacker, const GameSession& target, short rangeX, short rangeY) const
{
    const int diffX = static_cast<int>(target.X) - static_cast<int>(attacker.X);
    const int diffY = static_cast<int>(target.Y) - static_cast<int>(attacker.Y);

    if (std::abs(diffY) > rangeY)
        return false;

    const bool attackLeft = attacker.Direction == dfPACKET_MOVE_DIR_LL || attacker.Direction == dfPACKET_MOVE_DIR_LU || attacker.Direction == dfPACKET_MOVE_DIR_LD;
    const bool attackRight = attacker.Direction == dfPACKET_MOVE_DIR_RR || attacker.Direction == dfPACKET_MOVE_DIR_RU || attacker.Direction == dfPACKET_MOVE_DIR_RD;

    if (attackLeft)
        return diffX <= 0 && diffX >= -rangeX;

    if (attackRight)
        return diffX >= 0 && diffX <= rangeX;

    return false;
}

void CGameServer::SendDamage(const GameSession& attacker, const GameSession& damaged)
{
    st_PACKET_SC_DAMAGE packet{};
    packet.header = MakePacketHeader<st_PACKET_SC_DAMAGE>(dfPACKET_SC_DAMAGE);
    packet.attackId = attacker.PlayerId;
    packet.damageId = damaged.PlayerId;
    packet.damageHp = damaged.HP;
    SendBroadcast(nullptr, packet);
}

void CGameServer::ProcessDeadPlayer(CSession* session)
{
    if (session == nullptr)
        return;

    GameSession* deadPlayer = FindGameSession(session);

    if (deadPlayer == nullptr || !deadPlayer->Active)
        return;

    CLogger::Info("Player dead. session=%llu, player=%d", session->GetSessionId(), deadPlayer->PlayerId);
    Disconnect(session);
}

inline const AttackGameData* CGameServer::GetAttackData(std::uint8_t attackType) const
{
    switch (attackType)
    {
    case dfPACKET_CS_ATTACK1:
        return &m_GameData.Attack1;

    case dfPACKET_CS_ATTACK2:
        return &m_GameData.Attack2;

    case dfPACKET_CS_ATTACK3:
        return &m_GameData.Attack3;

    default:
        return nullptr;
    }
}

inline bool CGameServer::IsAttackLocked(const GameSession& gameSession) const
{
    return m_FixedTick < gameSession.AttackLockUntilTick;
}

inline bool CGameServer::ValidatePacketSize(const char* packet, int packetSize, int expectedSize)
{
    if (packet == nullptr || packetSize != expectedSize)
        return false;

    const st_PACKET_HEADER* header = reinterpret_cast<const st_PACKET_HEADER*>(packet);
    return static_cast<int>(sizeof(st_PACKET_HEADER)) + header->bySize == expectedSize;
}

inline bool CGameServer::ValidateDirection(std::uint8_t direction) const
{
    return direction <= dfPACKET_MOVE_DIR_LD;
}

inline bool CGameServer::IsClientPositionInErrorRange(const GameSession& gameSession, short clientX, short clientY) const
{
    const int diffX = std::abs(static_cast<int>(clientX) - static_cast<int>(gameSession.X));
    const int diffY = std::abs(static_cast<int>(clientY) - static_cast<int>(gameSession.Y));
    return diffX <= m_GameData.Move.ErrorRange && diffY <= m_GameData.Move.ErrorRange;
}

inline short CGameServer::ClampX(short x) const
{
    if (x <= m_GameData.World.MoveLeft)
        return static_cast<short>(m_GameData.World.MoveLeft + 1);

    if (x >= m_GameData.World.MoveRight)
        return static_cast<short>(m_GameData.World.MoveRight - 1);

    return x;
}

inline short CGameServer::ClampY(short y) const
{
    if (y <= m_GameData.World.MoveTop)
        return static_cast<short>(m_GameData.World.MoveTop + 1);

    if (y >= m_GameData.World.MoveBottom)
        return static_cast<short>(m_GameData.World.MoveBottom - 1);

    return y;
}

inline std::uint32_t CGameServer::GetClientTickMs() const
{
    if (m_Config.ClientFPS <= 0)
        return 20;

    const std::uint32_t tick = static_cast<std::uint32_t>(1000 / m_Config.ClientFPS);
    return tick == 0 ? 1 : tick;
}

inline int CGameServer::GetActivePlayerCount() const
{
    return m_ActiveGameSessionCount;
}
