
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

    COORD preCussorPos; // 기존 플레이어 위치
    COORD curCussorPos; // 현재 플레이어 위치

    COORD enemyWorldBasis = { 10, 2 };

    SMALL_RECT CussorMovableRect = { 5, 5, 30, 30 }; // @SEE StartGame()

    const int CussorMoveSpeed = 200;


    // 노가다로-0- 적을 만들어 봅시다.
    const int ENEMY_CNT = 10;
    struct Enemy
    {
        COORD localPos;
        char  character;
    };

    Enemy consoleEnemy[ENEMY_CNT];
};

void Clamp(short& n, short min, short max) // 레퍼런스 타입에 대해 배워 봅시다.
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

    // 노가다로 그리는 적
    for (int i = 0; i < global::ENEMY_CNT; i++)
    {
        x = global::enemyWorldBasis.X + global::consoleEnemy[i].localPos.X;
        y = global::enemyWorldBasis.Y + global::consoleEnemy[i].localPos.Y;

        render::ScreenDraw(x, y, global::consoleEnemy[i].character);
    }
}

void UpdateCussorPosition()
{
    global::preCussorPos = global::curCussorPos; // 현재 위치 경신 전에 일단, 저장. 구조체를 쓰면 이런게 편한겁니다. :)

    if (global::input::IsEscapeCmdOn())
    {
        global::input::Set(global::input::ESCAPE_KEY_INDEX, false);

        //TODO: 게임 화면 전환이나 종료 처리

        return; // 다른 키 입력을 신경 쓸 필요가 없어요.
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
    // 키 입력과 화면 출력과 게임 로직 갱신을 분리해서 생각해 봅시다.
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

    render::DrawBorder(); // 벽을 그려 놓자!

    // 플레이어 시작 위치 설정
    global::preCussorPos.X = global::CussorMovableRect.Left + (global::CussorMovableRect.Left + global::CussorMovableRect.Right) / 2;
    global::preCussorPos.Y = global::CussorMovableRect.Bottom - 2;

    global::curCussorPos.X = global::preCussorPos.X;
    global::curCussorPos.Y = global::preCussorPos.Y;

    // 노가다로 만드는 적
    for (int i = 0; i < global::ENEMY_CNT; i++)
    {
        global::consoleEnemy[i].character = 'A' + i;
        global::consoleEnemy[i].localPos.X = i * 10;
        global::consoleEnemy[i].localPos.Y = 0; // Y 는 고정.
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



