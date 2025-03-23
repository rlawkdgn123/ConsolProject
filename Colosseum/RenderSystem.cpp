#include <stdio.h>
#include "renderSystem.h"
#include "GameSystem.h"


void global::render::DrawPlayer()
{
    render::ScreenDraw(game::curPlayerPos.X, game::curPlayerPos.Y, 'V');
}
void global::render::InitScreen()
{
    // 화면 버퍼 2개를 만든다.
    hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

    // 깜박이는 커서를 좀 진정 시키자.
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.bVisible = 0; // 커서를 보일지 말지 결정(0이면 안보임, 0제외 숫자 값이면 보임)
    cursorInfo.dwSize = 1; // 커서의 크기를 결정 (1~100 사이만 가능)
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);


    // 콘솔창 크기를 가져 오자.
    // 지정할 수도 있는데 학습 용으로 그냥, 가져오는 예제로 작성.
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    consoleScreenSize.Left = csbi.srWindow.Left;
    consoleScreenSize.Right = csbi.srWindow.Right;
    consoleScreenSize.Bottom = csbi.srWindow.Bottom;
    consoleScreenSize.Top = csbi.srWindow.Top;

    consoleScreenX = csbi.dwSize.X;
    consoleScreenY = csbi.dwSize.Y;

    // 실제 갱신할 화면 영역을 지정하자. 콘솔 크기 안쪽 사각형이라고 생각하면 됩니다.
    updateScreenSize.Left = consoleScreenSize.Left + 2;
    updateScreenSize.Right = consoleScreenSize.Right - 2;
    updateScreenSize.Bottom = consoleScreenSize.Bottom - 2;
    updateScreenSize.Top = consoleScreenSize.Top + 2;

    updateScreenX = consoleScreenX - 4;
    updateScreenY = consoleScreenY - 4;
}
SMALL_RECT global::render::GetPlayerMovableRect()
{
    return updateScreenSize;
}
        // 콘솔이다 보니 픽셀 단위가아니라 라인 단위...한 글자씩 그려(?) 주렵니다.
void global::render::ScreenDraw(int x, int y, const char c)
{
    DWORD dw;
    COORD Cur = { x, y };
    char buffer[10];
    sprintf_s(buffer, "%c", c);

    SetConsoleCursorPosition(GetScreenHandle(), Cur);

    WriteFile(GetScreenHandle(), buffer, 1, &dw, NULL);
}

HANDLE global::render::GetScreenHandle()
{
    int index = (render::bScreenIndex ? 1 : 0);

    return render::hScreen[index];
}

void global::render::ScreenDraw(int x, int y, const char pStr)
{
    DWORD dw;
    COORD Cur = { x, y };

    SetConsoleCursorPosition(GetScreenHandle(), Cur);

    WriteFile(GetScreenHandle(), pStr, strlen(pStr), &dw, NULL);
}

