#include <stdio.h>
#include "renderSystem.h"
#include "GameSystem.h"


void global::render::DrawPlayer()
{
    render::ScreenDraw(game::curPlayerPos.X, game::curPlayerPos.Y, 'V');
}
void global::render::InitScreen()
{
    // ȭ�� ���� 2���� �����.
    hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

    // �����̴� Ŀ���� �� ���� ��Ű��.
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.bVisible = 0; // Ŀ���� ������ ���� ����(0�̸� �Ⱥ���, 0���� ���� ���̸� ����)
    cursorInfo.dwSize = 1; // Ŀ���� ũ�⸦ ���� (1~100 ���̸� ����)
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);


    // �ܼ�â ũ�⸦ ���� ����.
    // ������ ���� �ִµ� �н� ������ �׳�, �������� ������ �ۼ�.
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    consoleScreenSize.Left = csbi.srWindow.Left;
    consoleScreenSize.Right = csbi.srWindow.Right;
    consoleScreenSize.Bottom = csbi.srWindow.Bottom;
    consoleScreenSize.Top = csbi.srWindow.Top;

    consoleScreenX = csbi.dwSize.X;
    consoleScreenY = csbi.dwSize.Y;

    // ���� ������ ȭ�� ������ ��������. �ܼ� ũ�� ���� �簢���̶�� �����ϸ� �˴ϴ�.
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
        // �ܼ��̴� ���� �ȼ� �������ƴ϶� ���� ����...�� ���ھ� �׷�(?) �ַƴϴ�.
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

