#include "RenderSystem.h"
#include "GameSystem.h"
#include "TimeSystem.h"
void game::StartGame()
{
    // �����̴� Ŀ���� �� ���� ��Ű��.
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.bVisible = 0; // Ŀ���� ������ ���� ����(0�̸� �Ⱥ���, 0���� ���� ���̸� ����)
    cursorInfo.dwSize = 1; // Ŀ���� ũ�⸦ ���� (1~100 ���̸� ����)

    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    // �ܼ�â ũ�⸦ ���� ����
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    consoleScreenSize.Left = csbi.srWindow.Left;
    consoleScreenSize.Right = csbi.srWindow.Right;
    consoleScreenSize.Bottom = csbi.srWindow.Bottom;
    consoleScreenSize.Top = csbi.srWindow.Top;

    // �÷��̾ �̵� ������ ������ ���� ����. 
    playerMovableRect.Left = consoleScreenSize.Left + 2;
    playerMovableRect.Right = consoleScreenSize.Right - 2;
    playerMovableRect.Bottom = consoleScreenSize.Bottom - 2;
    playerMovableRect.Top = consoleScreenSize.Top + 2;

    DrawMovableRect(); // ���� �׷� ����!

    // �÷��̾� ���� ��ġ ����
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

