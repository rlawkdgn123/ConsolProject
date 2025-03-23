
#include <stdio.h>
#include <windows.h>
#include <random>

#include "InputSystem.h" // 기능 별로 모듈화를 한다는 개념에 대해 생각해 봅시다!
#include "RenderSystem.h"
#include "MainSystem.h"


void Clamp(short& n, short min, short max) // 레퍼런스 타입에 대해 배워 봅시다.
{
    if (n < min) n = min;
    if (n > max) n = max;
}

void DrawPlayer()
{
    render::ScreenDraw(global::curPlayerPos.X, global::curPlayerPos.Y, 'V');
}

//void DrawEnemy()
//{
//    int x = 0; int y = 0;
//
//    // 노가다로 그리는 적
//    for (int i = 0; i < global::ENEMY_CNT; i++)
//    {
//        x = global::enemyWorldBasis.X + global::consoleEnemy[i].localPos.X;
//        y = global::enemyWorldBasis.Y + global::consoleEnemy[i].localPos.Y;
//
//        render::ScreenDraw(x, y, global::consoleEnemy[i].character);
//    }
//}


void UpdatePlayerPosition()
{
    global::prePlayerPos = global::curPlayerPos; // 현재 위치 경신 전에 일단, 저장. 구조체를 쓰면 이런게 편한겁니다. :)

    if (global::input::IsEscapeCmdOn())
    {
        global::input::Set(global::input::ESCAPE_KEY_INDEX, false);

        //TODO: 게임 화면 전환이나 종료 처리

        return; // 다른 키 입력을 신경 쓸 필요가 없어요.
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
    // 키 입력과 화면 출력과 게임 로직 갱신을 분리해서 생각해 봅시다.
    static ULONGLONG elapsedTime;

    elapsedTime += global::time::GetDeltaTime(); // 현재시각 - 과거시각(델타타임) 매번 더해주기

    while (elapsedTime >= global::playerMoveSpeed)
    {
        UpdatePlayerPosition(); // 플레이어 위치 갱신

        elapsedTime -= global::playerMoveSpeed; // 플레이어 속도만큼 델타타임 당 이동량 감소
    }
}

//void UpdateEnemy()
//{
//    if (global::enemyWorldBasis.Y == global::playerMovableRect.Bottom)
//    {
//        global::enemyWorldBasis.Y = global::playerMovableRect.Top;
//
//        return;
//    }
//
//    global::enemyWorldBasis.Y++;
//
//    Clamp(global::enemyWorldBasis.Y, global::playerMovableRect.Top, global::playerMovableRect.Bottom);
//}

void StartGame()
{
    render::InitScreen();

    global::playerMovableRect = render::GetPlayerMovableRect();

    //render::DrawBorder(); // 벽을 그려 놓자!

    global::introFlag = true; // 인트로 시작 Flag

    // 플레이어 시작 위치 설정
    global::prePlayerPos.X = global::playerMovableRect.Left + (global::playerMovableRect.Left + global::playerMovableRect.Right) / 2;
    global::prePlayerPos.Y = global::playerMovableRect.Bottom - 2;

    global::curPlayerPos.X = global::prePlayerPos.X;
    global::curPlayerPos.Y = global::prePlayerPos.Y;

    //// 노가다로 만드는 적
    //for (int i = 0; i < global::ENEMY_CNT; i++)
    //{
    //    global::consoleEnemy[i].character = 'A' + i;
    //    global::consoleEnemy[i].localPos.X = i * 10;
    //    global::consoleEnemy[i].localPos.Y = 0; // Y 는 고정.
    //}

    DrawPlayer();
    //DrawEnemy();
}

void EndGame()
{
    render::ScreenRelease();
}

void ProcessInput()
{
    global::input::UpdateInput(); // 입력받기
}

void PrintCountsPerSecond();
void PrintPlayerPostion();

void Render()
{
    render::ScreenClear();

    PrintCountsPerSecond();

    PrintPlayerPostion();

    DrawPlayer();


    //DrawEnemy();

    //render::DrawBorder();
    render::DrawStage(global::stage);

    render::ScreenFlipping();
}

bool IsGameRun()
{
    return true;
}

void FixeUpdate()
{
    static ULONGLONG elapsedTime;

    elapsedTime += global::time::GetDeltaTime();

    while (elapsedTime >= 1000) //1초
    {
        global::time::fixedUpdateCount += 1;

        elapsedTime -= 1000;

        //UpdateEnemy();
    }
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

int main()
{
    global::time::InitTime();

    StartGame();

    while (IsGameRun())
    {
        global::time::UpdateTime();

        ProcessInput();
        FixeUpdate(); // 일단 FIXED 안쓸듯

        Update();
        Render();
    }

    EndGame();

    return 0;
}

