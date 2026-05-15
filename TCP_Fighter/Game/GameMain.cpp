#include "GameServer.h"
#include "GameDefine.h"
#include <Windows.h>

static void HideConsoleCursor()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cursorInfo{};
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

int main()
{
    HideConsoleCursor();

    CGameServer server;

    if (!server.Init(dfDEFAULT_CONFIG_PATH))
        return 1;

    server.Run();
    return 0;
}
