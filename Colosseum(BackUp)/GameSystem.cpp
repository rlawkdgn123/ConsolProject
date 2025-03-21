
#include <stdio.h>
#include <windows.h>
#include <random>
#include "GameSystem.h"

namespace global
{
    namespace time
    {
        ULONGLONG previousTime;
        ULONGLONG currentTime;
        ULONGLONG deltaTime;

        int updateCount;
        int fixedUpdateCount;

        void InitTime()
        {
            currentTime = previousTime = GetTickCount64();
        }

        void UpdateTime()
        {
            previousTime = currentTime;
            currentTime = GetTickCount64();

            deltaTime = currentTime - previousTime;
        }

        ULONGLONG GetDeltaTime()
        {
            return deltaTime;
        }

    };

    COORD preCussorPos; // ���� �÷��̾� ��ġ
    COORD curCussorPos; // ���� �÷��̾� ��ġ

    COORD enemyWorldBasis = { 10, 2 };

    SMALL_RECT CussorMovableRect = { 5, 5, 30, 30 }; // @SEE StartGame()

    const int CussorMoveSpeed = 200;


    // �밡�ٷ�-0- ���� ����� ���ô�.
    const int ENEMY_CNT = 10;
    struct Enemy
    {
        COORD localPos;
        char  character;
    };

    Enemy consoleEnemy[ENEMY_CNT];
};

void Clamp(short& n, short min, short max) // ���۷��� Ÿ�Կ� ���� ��� ���ô�.
{
    if (n < min) n = min;
    if (n > max) n = max;
}

void DrawCussor()
{
    render::ScreenDraw(global::curCussorPos.X, global::curCussorPos.Y, '@');
}

void DrawEnemy()
{
    int x = 0; int y = 0;

    // �밡�ٷ� �׸��� ��
    for (int i = 0; i < global::ENEMY_CNT; i++)
    {
        x = global::enemyWorldBasis.X + global::consoleEnemy[i].localPos.X;
        y = global::enemyWorldBasis.Y + global::consoleEnemy[i].localPos.Y;

        render::ScreenDraw(x, y, global::consoleEnemy[i].character);
    }
}

void UpdateCussorPosition()
{
    global::preCussorPos = global::curCussorPos; // ���� ��ġ ��� ���� �ϴ�, ����. ����ü�� ���� �̷��� ���Ѱ̴ϴ�. :)

    if (global::input::IsEscapeCmdOn())
    {
        global::input::Set(global::input::ESCAPE_KEY_INDEX, false);

        //TODO: ���� ȭ�� ��ȯ�̳� ���� ó��

        return; // �ٸ� Ű �Է��� �Ű� �� �ʿ䰡 �����.
    }

    if (global::input::IsLeftCmdOn())
    {
        global::input::Set(global::input::USER_CMD_LEFT, false);

        global::curCussorPos.X--;
        Clamp(global::curCussorPos.X, global::CussorMovableRect.Left, global::CussorMovableRect.Right);
    }

    if (global::input::IsRightCmdOn())
    {
        global::input::Set(global::input::USER_CMD_RIGHT, false);

        global::curCussorPos.X++;
        Clamp(global::curCussorPos.X, global::CussorMovableRect.Left, global::CussorMovableRect.Right);
    }
}

void UpdatePlayer()
{
    // Ű �Է°� ȭ�� ��°� ���� ���� ������ �и��ؼ� ������ ���ô�.
    static ULONGLONG elapsedTime;

    elapsedTime += global::time::GetDeltaTime();

    while (elapsedTime >= global::CussorMoveSpeed)
    {
        UpdateCussorPosition();

        elapsedTime -= global::CussorMoveSpeed;
    }
}

void UpdateEnemy()
{
    if (global::enemyWorldBasis.Y == global::CussorMovableRect.Bottom)
    {
        global::enemyWorldBasis.Y = global::CussorMovableRect.Top;

        return;
    }

    global::enemyWorldBasis.Y++;

    Clamp(global::enemyWorldBasis.Y, global::CussorMovableRect.Top, global::CussorMovableRect.Bottom);
}

void StartGame()
{
    render::InitScreen();

    global::CussorMovableRect = render::GetCussorMovableRect();

    render::DrawBorder(); // ���� �׷� ����!

    // �÷��̾� ���� ��ġ ����
    global::preCussorPos.X = global::CussorMovableRect.Left + (global::CussorMovableRect.Left + global::CussorMovableRect.Right) / 2;
    global::preCussorPos.Y = global::CussorMovableRect.Bottom - 2;

    global::curCussorPos.X = global::preCussorPos.X;
    global::curCussorPos.Y = global::preCussorPos.Y;

    // �밡�ٷ� ����� ��
    for (int i = 0; i < global::ENEMY_CNT; i++)
    {
        global::consoleEnemy[i].character = 'A' + i;
        global::consoleEnemy[i].localPos.X = i * 10;
        global::consoleEnemy[i].localPos.Y = 0; // Y �� ����.
    }

    DrawCussor();

    DrawEnemy();
}

void EndGame()
{
    render::ScreenRelease();
}

void ProcessInput()
{
    global::input::UpdateInput();
}

void PrintCountsPerSecond();
void PrintCussorPostion();

void Render()
{
    render::ScreenClear();

    PrintCountsPerSecond();

    PrintCussorPostion();

    DrawCussor();

    DrawEnemy();

    render::DrawBorder();

    render::ScreenFlipping();
}

bool IsGameRun()
{
    return true;
}

void Update()
{
    global::time::updateCount += 1;

    UpdatePlayer();

}

void PrintCussorPostion()
{
    char buffer[64] = "";
    sprintf_s(buffer, "Cussor Position (%d, %d)", global::curCussorPos.X, global::curCussorPos.Y);

    render::ScreenDraw(50, 0, buffer);
}

void PrintCountsPerSecond()
{
    static ULONGLONG elapsedTime;

    elapsedTime += global::time::GetDeltaTime();
    if (elapsedTime >= 1000)
    {
        char buffer[64] = "";

        sprintf_s(buffer, "updateCount : %d fixedUpdateCount : %d  delta %2d\n",
            global::time::updateCount, global::time::fixedUpdateCount, global::time::GetDeltaTime());

        render::ScreenDraw(0, 0, buffer);

        elapsedTime = 0;
        global::time::updateCount = 0;
        global::time::fixedUpdateCount = 0;
    }
}



