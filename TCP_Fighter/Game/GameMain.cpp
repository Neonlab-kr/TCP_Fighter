#include "GameServer.h"
#include "GameDefine.h"

#include <Windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

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
    timeBeginPeriod(1);
    HideConsoleCursor();

    int result = 0;

    {
        CGameServer server;

        if (!server.Init(dfDEFAULT_CONFIG_PATH))
        {
            timeEndPeriod(1);
            return 1;
        }

        server.Run();
    }

    timeEndPeriod(1);
    return result;
}
