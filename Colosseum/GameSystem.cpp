#include "RenderSystem.h"
#include "GameSystem.h"
#include "TimeSystem.h"
void game::StartGame()
{
    // 깜박이는 커서를 좀 진정 시키자.
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.bVisible = 0; // 커서를 보일지 말지 결정(0이면 안보임, 0제외 숫자 값이면 보임)
    cursorInfo.dwSize = 1; // 커서의 크기를 결정 (1~100 사이만 가능)

    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    // 콘솔창 크기를 가져 오고
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    consoleScreenSize.Left = csbi.srWindow.Left;
    consoleScreenSize.Right = csbi.srWindow.Right;
    consoleScreenSize.Bottom = csbi.srWindow.Bottom;
    consoleScreenSize.Top = csbi.srWindow.Top;

    // 플레이어가 이동 가능한 영역을 설정 하자. 
    playerMovableRect.Left = consoleScreenSize.Left + 2;
    playerMovableRect.Right = consoleScreenSize.Right - 2;
    playerMovableRect.Bottom = consoleScreenSize.Bottom - 2;
    playerMovableRect.Top = consoleScreenSize.Top + 2;

    DrawMovableRect(); // 벽을 그려 놓자!

    // 플레이어 시작 위치 설정
    prePlayerPos.X = 10;
    prePlayerPos.Y = 10;
    curPlayerPos.X = 10;
    curPlayerPos.Y = 10;

    DrawPlayer(false);
}

bool game::IsGameRun()
{
    return true;
}

void game::EndGame()
{
}

void game::GotoXY(int x, int y)
{
    {
        COORD Cur = { x, y };

        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
    }
}

