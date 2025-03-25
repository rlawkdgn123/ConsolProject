#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <random>
#include "Define.h"
#include "InputSystem.h" // 기능 별로 모듈화를 한다는 개념에 대해 생각해 봅시다!
#include "RenderSystem.h"
#include "Player.h"
namespace global {
    int mSecPerFrame = 1000 / 25; // 40
    COORD prePlayerPos; // 기존 플레이어 위치
    COORD curPlayerPos; // 현재 플레이어 위치
    int saveXPos[5] = { 0, };

    COORD enemyWorldBasis = { 10, 2 };

    SMALL_RECT playerMovableRect = { 5, 5, 30, 30 }; // @SEE StartGame()

    const int playerMoveSpeed = 200; // 플레이어 이동 속도

    bool cussorTP = true; // 커서 순간이동(T) / 연속이동(F) 여부

    // 노가다로-0- 적을 만들어 봅시다.
    /*const int ENEMY_CNT = 10;
    struct Enemy
    {
        COORD localPos;
        char  character;
    };
    Enemy consoleEnemy[ENEMY_CNT];*/
    namespace time
    {
        ULONGLONG previousTime;
        ULONGLONG currentTime;
        ULONGLONG deltaTime;

        int updateCount;
        int fixedUpdateCount;

        void InitTime()
        {
            currentTime = previousTime = GetTickCount64(); // 틱 카운트 비교로 시간 계산
        }
        void UpdateTime()
        {
            previousTime = currentTime;
            currentTime = GetTickCount64();

            deltaTime = currentTime - previousTime; // 델타 타임 구하기
        }
        ULONGLONG GetDeltaTime()
        {
            return deltaTime;
        }
    };
}

void Clamp(short& n, short min, short max) // 레퍼런스 타입에 대해 배워 봅시다.
{
    if (n < min) n = min;
    if (n > max) n = max;
}

void DrawPlayer()
{
    render::ScreenDraw(global::curPlayerPos.X, global::curPlayerPos.Y, '@');
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
void Choice(int* menuFlag, int* maxIndex) {
    static int preMenu = 0;

    switch (*menuFlag) {
    case TITLE:
        global::saveXPos[0] = POS1;
        global::saveXPos[1] = POS2;
        global::saveXPos[2] = POS3;
        *maxIndex = 3;
        global::curPlayerPos.Y = 40;
        break;
    case HEROCHOICE:
        global::saveXPos[0] = POS1;
        global::saveXPos[1] = POS2;
        global::saveXPos[2] = POS3;
        *maxIndex = 3;
        break;
    case MAIN:
        break;
    case BATTLE:
        break;
    case END:
        break;
    default:
        break;
    }

    preMenu = *menuFlag;
}
void UpdatePlayerPosition(int* menuFlag, int* index)
{
    static int maxIndex = 0;
    
    if(*menuFlag == TITLE && global::saveXPos[0] == 0) // 처음 한 번만 호출
        Choice(menuFlag, &maxIndex);
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

            if(*index > 0)
                global::curPlayerPos.X = global::saveXPos[--(*index)];
        }
        if (global::input::IsRightCmdOn())
        {
            global::input::Set(global::input::USER_CMD_RIGHT, false);

            if (*index < maxIndex - 1)
                global::curPlayerPos.X = global::saveXPos[++(*index)];
        }
        if (global::input::IsSpaceCmdOn())
        {
            global::input::Set(global::input::USER_CMD_SPACE, false);

            Choice(menuFlag, &maxIndex);
        }
    }

//void UpdatePlayer(int* menuFlag)
//{
//    // 키 입력과 화면 출력과 게임 로직 갱신을 분리해서 생각해 봅시다.
//    static ULONGLONG elapsedTime;
//
//    elapsedTime += global::time::GetDeltaTime();
//
//
//    /*while (elapsedTime >= global::playerMoveSpeed)
//    {
//        UpdatePlayerPosition();
//
//        elapsedTime -= global::playerMoveSpeed;
//    }*/
//}

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

    //render::DrawBorder(); // 벽을 그려 놓자!

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

    //DrawPlayer();

    //DrawEnemy();

    
}

void EndGame()
{
    render::ScreenRelease();
}

void ProcessInput() // 인풋 종류 정하기
{
    global::input::InputState();
    //global::input::UpdateInput();
}

void PrintCountsPerSecond();
void PrintPlayerPostion();

void Render(int* menuFlag, int* curIndex)
{
    render::ScreenClear();

    PrintCountsPerSecond();

    PrintPlayerPostion();

    //DrawPlayer();

    //DrawEnemy();
    render::DrawGames(menuFlag, &global::curPlayerPos, curIndex);
    //render::DrawBorder();

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

void Update(int* menuFlag, int* curIndex)
{
    
    global::time::updateCount += 1;

    if (global::input::IsSpaceCmdOn())
    {
        if (*menuFlag == TITLE)
        {
            switch (global::curPlayerPos.X)
            {
            case POS1: *menuFlag = HEROCHOICE;
                break;
            case POS2: *menuFlag = HOWTOPLAY;
                break;
            case POS3: *menuFlag = GAMEINFO;
                break;
            default:
                break;
            }
        }
        else if (*menuFlag == HEROCHOICE)
        {
            switch (global::curPlayerPos.X)
            {
            case POS1: *menuFlag = HEROCHOICE;
                break;
            case POS2: *menuFlag = HOWTOPLAY;
                break;
            case POS3: *menuFlag = GAMEINFO;
                break;
            default:
                break;
            }
        }
        else if (*menuFlag == MAIN)
        {

        }
        else if (*menuFlag == BATTLE)
        {

        }
        else if (*menuFlag == END)
        {

        }
    }

    UpdatePlayerPosition(menuFlag, curIndex);

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

    int menuFlag = TITLE;
    int choiceIndex = 0;
    PLAYER job[JOB];
    PLAYER player;
    PLAYER enemy[JOB - 1];

    StartGame();
    player::SetPlayer(job);

    while (IsGameRun())
    {
        global::time::UpdateTime();

        static ULONGLONG elapsedTime = 0;

        elapsedTime += global::time::GetDeltaTime();
        if (elapsedTime > global::mSecPerFrame)
        {
            ProcessInput();
            FixeUpdate();

            Update(&menuFlag, &choiceIndex);
            Render(&menuFlag, &choiceIndex);
            elapsedTime -= global::mSecPerFrame;
        }
    }

    EndGame();

    return 0;
}

