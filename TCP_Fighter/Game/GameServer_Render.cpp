#include "GameServer.h"

#include <Windows.h>
#include <cstdio>
#include <cstring>

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

    PrintClearLine(console, consoleWidth, "Select TCP Game Server");

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

    std::snprintf(lineBuffer, sizeof(lineBuffer), "Sessions    : %d / %d", GetSessionCount(), GetMaxSession());
    PrintClearLine(console, consoleWidth, lineBuffer);

    std::snprintf(lineBuffer, sizeof(lineBuffer), "Players     : %d", m_ActiveGameSessionCount);
    PrintClearLine(console, consoleWidth, lineBuffer);

    std::snprintf(lineBuffer, sizeof(lineBuffer), "Time        : %u", m_CurrentTimeMs);
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
            IsAttackLocked(p, m_CurrentTimeMs) ? "Y" : "N");

        PrintClearLine(console, consoleWidth, lineBuffer);
        ++line;
    }

    for (; line < m_GameData.ServerDebug.RenderPlayerLineMax + m_GameData.ServerDebug.RenderClearExtraLineCount; ++line)
        PrintClearLine(console, consoleWidth, "");
}
