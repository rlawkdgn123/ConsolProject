
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

    COORD prePlayerPos; // ���� �÷��̾� ��ġ
    COORD curPlayerPos; // ���� �÷��̾� ��ġ

    COORD enemyWorldBasis = { 10, 2 };

    SMALL_RECT playerMovableRect = { 5, 5, 30, 30 }; // @SEE StartGame()

    const int playerMoveSpeed = 200;


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

void DrawPlayer()
{
    render::ScreenDraw(global::curPlayerPos.X, global::curPlayerPos.Y, '@');
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

void UpdatePlayerPosition()
{
    global::prePlayerPos = global::curPlayerPos; // ���� ��ġ ��� ���� �ϴ�, ����. ����ü�� ���� �̷��� ���Ѱ̴ϴ�. :)

    if (global::input::IsEscapeCmdOn())
    {
        global::input::Set(global::input::ESCAPE_KEY_INDEX, false);

        //TODO: ���� ȭ�� ��ȯ�̳� ���� ó��

        return; // �ٸ� Ű �Է��� �Ű� �� �ʿ䰡 �����.
    }

    if (global::input::IsLeftCmdOn())
    {
        global::input::Set(global::input::USER_CMD_LEFT, false);

        global::curPlayerPos.X--;
        Clamp(global::curPlayerPos.X, global::playerMovableRect.Left, global::playerMovableRect.Right);
    }

    if (global::input::IsRightCmdOn())
    {
        global::input::Set(global::input::USER_CMD_RIGHT, false);

        global::curPlayerPos.X++;
        Clamp(global::curPlayerPos.X, global::playerMovableRect.Left, global::playerMovableRect.Right);
    }
}

void UpdatePlayer()
{
    // Ű �Է°� ȭ�� ��°� ���� ���� ������ �и��ؼ� ������ ���ô�.
    static ULONGLONG elapsedTime;

    elapsedTime += global::time::GetDeltaTime();

    while (elapsedTime >= global::playerMoveSpeed)
    {
        UpdatePlayerPosition();

        elapsedTime -= global::playerMoveSpeed;
    }
}

void UpdateEnemy()
{
    if (global::enemyWorldBasis.Y == global::playerMovableRect.Bottom)
    {
        global::enemyWorldBasis.Y = global::playerMovableRect.Top;

        return;
    }

    global::enemyWorldBasis.Y++;

    Clamp(global::enemyWorldBasis.Y, global::playerMovableRect.Top, global::playerMovableRect.Bottom);
}

void StartGame()
{
    render::InitScreen();

    global::playerMovableRect = render::GetPlayerMovableRect();

    render::DrawBorder(); // ���� �׷� ����!

    // �÷��̾� ���� ��ġ ����
    global::prePlayerPos.X = global::playerMovableRect.Left + (global::playerMovableRect.Left + global::playerMovableRect.Right) / 2;
    global::prePlayerPos.Y = global::playerMovableRect.Bottom - 2;

    global::curPlayerPos.X = global::prePlayerPos.X;
    global::curPlayerPos.Y = global::prePlayerPos.Y;

    // �밡�ٷ� ����� ��
    for (int i = 0; i < global::ENEMY_CNT; i++)
    {
        global::consoleEnemy[i].character = 'A' + i;
        global::consoleEnemy[i].localPos.X = i * 10;
        global::consoleEnemy[i].localPos.Y = 0; // Y �� ����.
    }

    DrawPlayer();

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
void PrintPlayerPostion();

void Render()
{
    render::ScreenClear();

    PrintCountsPerSecond();

    PrintPlayerPostion();

    DrawPlayer();

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

void PrintPlayerPostion()
{
    char buffer[64] = "";
    sprintf_s(buffer, "Player Position (%d, %d)", global::curPlayerPos.X, global::curPlayerPos.Y);

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



