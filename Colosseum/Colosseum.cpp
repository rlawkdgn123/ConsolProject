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
    COORD curPlayerPos = { 20, 40 }; // 현재 플레이어 위치
    int saveXPos[5] = { 0, };

    COORD enemyWorldBasis = { 10, 2 };

    SMALL_RECT playerMovableRect = { 5, 5, 30, 30 }; // @SEE StartGame()

    const int playerMoveSpeed = 200; // 플레이어 이동 속도

    bool cussorTP = true; // 커서 순간이동(T) / 연속이동(F) 여부

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

    namespace player
    {
        PLAYER job[JOBCOUNT];
        PLAYER player;
        PLAYER enemy[JOBCOUNT - 1];
        int current_enemy;
        bool isPlayerTurn;
        bool isEnemyTurn;
        bool isUseAttack;
        bool isUseSkill;
        bool isUseItem;
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
        global::saveXPos[0] = POS1;
        global::saveXPos[1] = POS2;
        *maxIndex = 2;
        break;
    case BATTLE:
        global::saveXPos[0] = POS1;
        global::saveXPos[1] = POS2;
        global::saveXPos[2] = POS3;
        *maxIndex = 3;
        break;
    case BATTLE_SKILL:
        global::saveXPos[0] = POS1;
        global::saveXPos[1] = POS2;
        global::saveXPos[2] = POS3;
        *maxIndex = 3;
        break;
    case BATTLE_ITEM:
        global::saveXPos[0] = POS1;
        global::saveXPos[1] = POS2;
        global::saveXPos[2] = POS3;
        *maxIndex = 3;
        break;
    case BATTLE_END:
        global::saveXPos[0] = POS1;
        global::saveXPos[1] = POS2;
        global::saveXPos[2] = POS3;
        *maxIndex = 3;
        break;
    case END:
        global::saveXPos[0] = POS1;
        *maxIndex = 1;
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
            

            Choice(menuFlag, &maxIndex);
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

    //render::DrawBorder(); // 벽을 그려 놓자!

    // 플레이어 시작 위치 설정
    global::prePlayerPos.X = 30;
    global::prePlayerPos.Y = 40;

    global::curPlayerPos.X = global::prePlayerPos.X;
    global::curPlayerPos.Y = global::prePlayerPos.Y;

    //DrawPlayer();

    //DrawEnemy();

    player::SetPlayer(global::player::job);

    global::player::player.hp = 1;
    global::player::enemy[0].hp = 1;
    global::player::enemy[1].hp = 1;
    global::player::isPlayerTurn = true;
    global::player::isEnemyTurn = false;
    global::player::isUseSkill = false;
}

void EndGame()
{
    render::ScreenRelease();
}

void ProcessInput() // 인풋 종류 정하기
{
    global::input::InputState();
}

void PrintCountsPerSecond();
void PrintPlayerPostion();

void Render(int* menuFlag, int* curIndex)
{
    static ULONGLONG elapsedTime;

    render::ScreenClear();

    PrintCountsPerSecond();

    PrintPlayerPostion();

    //DrawPlayer();

    //DrawEnemy();
    render::DrawGames(menuFlag, &global::curPlayerPos, curIndex, &global::player::current_enemy, &global::player::player, global::player::enemy);
    //render::DrawBorder();
    if (global::player::isUseAttack || global::player::isUseItem || global::player::isUseSkill)
    {
        elapsedTime += global::time::GetDeltaTime();
        if (elapsedTime > 1000)
        {

        }
    }

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
    if (global::player::isUseAttack || global::player::isUseItem || global::player::isUseSkill)
        return;

    global::time::updateCount += 1;

    if (global::player::player.hp <= 0)
    {
        *menuFlag = END;
    }
    else if (global::player::enemy[global::player::current_enemy].hp <= 0)
    {
        global::player::enemy[global::player::current_enemy].isDied = true;

        if (global::player::enemy[0].isDied && global::player::enemy[1].isDied)
        {
            *menuFlag = END;
        }
        else
        {
            if (global::player::current_enemy == 0) // 두 번째 적 강화
            {
                global::player::enemy[1].hp = 150;
                global::player::enemy[1].item[1].itemcount += 1;
            }
            else if (global::player::current_enemy == 1) // 두 번째 적 강화
            {
                global::player::enemy[0].hp = 150;
                global::player::enemy[0].item[1].itemcount += 1;
            }
            global::player::player.skillCount = 0;
            *menuFlag = BATTLE_END;
        }
    }
    
    if (global::player::player.state == STUN && global::player::isPlayerTurn)
    {
        global::player::player.state = NORMAL;
        global::player::isPlayerTurn = false;
        global::player::isEnemyTurn = true;
    }

    if (global::player::isPlayerTurn && !(global::player::isUseSkill))
    {
        if (global::input::IsSpaceCmdOn())
        {
            global::input::Set(global::input::USER_CMD_SPACE, false);
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
                case POS1:
                    global::player::player = global::player::job[WARRIOR];
                    global::player::job[WARRIOR].isPlayer = true;
                    player::SetEnemy(global::player::job, global::player::enemy);
                    *menuFlag = MAIN;
                    break;
                case POS2:
                    global::player::player = global::player::job[THIEF];
                    global::player::job[THIEF].isPlayer = true;
                    player::SetEnemy(global::player::job, global::player::enemy);
                    *menuFlag = MAIN;
                    break;
                case POS3:
                    global::player::player = global::player::job[WIZARD];
                    global::player::job[THIEF].isPlayer = true;
                    player::SetEnemy(global::player::job, global::player::enemy);
                    *menuFlag = MAIN;
                    break;
                default:
                    break;
                }
            }
            else if (*menuFlag == MAIN)
            {
                switch (global::curPlayerPos.X)
                {
                case POS1:
                    global::player::current_enemy = 0;
                    *menuFlag = BATTLE;
                    break;
                case POS2:
                    global::player::current_enemy = 1;
                    *menuFlag = BATTLE;
                    break;
                default:
                    break;
                }
            }
            else if (*menuFlag == BATTLE)
            {
                if (global::player::isPlayerTurn)
                {
                    switch (global::curPlayerPos.X)
                    {
                    case POS1: // 기본 공격
                        player::UseAttack(&global::player::player, &global::player::enemy[global::player::current_enemy]);
                        global::player::isUseAttack = true;
                        global::player::isPlayerTurn = false;
                        global::player::isEnemyTurn = true;
                        break;
                    case POS2: // 스킬창
                        *menuFlag = BATTLE_SKILL;
                        break;
                    case POS3: // 아이템 창
                        *menuFlag = BATTLE_ITEM;
                        break;
                    default:
                        break;
                    }
                }
            }
            else if (*menuFlag == BATTLE_SKILL)
            {
                switch (global::curPlayerPos.X)
                {
                case POS1: // 1번 스킬 사용
                    player::UseSkill(&global::player::player, &global::player::enemy[global::player::current_enemy], 0);
                    global::player::isUseSkill = true;
                    *menuFlag = BATTLE;
                    break;
                case POS2: // 2번 스킬 사용
                    player::UseSkill(&global::player::player, &global::player::enemy[global::player::current_enemy], 1);
                    global::player::isUseSkill = true;
                    *menuFlag = BATTLE;
                    break;
                case POS3: // 돌아가기
                    *menuFlag = BATTLE;
                default:
                    break;
                }
            }
            else if (*menuFlag == BATTLE_ITEM)
            {
                switch (global::curPlayerPos.X)
                {
                case POS1: // 포션 아이템 사용
                    if (global::player::player.item[0].itemcount > 0)
                    {
                        player::UseItem(&global::player::player, 0);
                        global::player::isUseItem = true;
                    }
                    break;
                case POS2: // 스턴 아이템 사용
                    if (global::player::player.item[0].itemcount > 0)
                    {
                        player::UseItem(&global::player::player, 1);
                        global::player::isUseItem = true;
                    }
                    break;
                case POS3: // 돌아가기
                    *menuFlag = BATTLE;
                default: break;
                }
            }
            else if (*menuFlag == BATTLE_END)
            {
                switch (global::curPlayerPos.X)
                {
                case POS1: //hp 50 회복
                    global::player::player.hp += 50;
                    *menuFlag = MAIN;
                    break;
                case POS2: // 도적이면 독 스택 데미지 최대치 +5, 나머지는 공격력 + 5
                    if (global::player::player.JOB == THIEF)
                    {
                        global::player::player.maxPoisonStk += 5;
                    }
                    else
                    {
                        global::player::player.atkDamage += 5;
                    }
                    *menuFlag = MAIN;
                    break;
                case POS3: // 스턴 1개 아이템 추가
                    global::player::player.item[1].itemcount += 1; 
                    *menuFlag = MAIN;
                    break;
                }
                }
            else if (*menuFlag == END)
            {
                switch (global::curPlayerPos.X)
                {
                case POS1:
                    *menuFlag = TITLE;
                    StartGame();
                }
            }
        }
    }

    if (global::player::isEnemyTurn && !(global::player::isUseSkill))
    {
        if (global::player::enemy[global::player::current_enemy].state == STUN)
        {
            global::player::enemy[global::player::current_enemy].state = NORMAL;
            global::player::isEnemyTurn = false;
            global::player::isPlayerTurn = true;
            
            return;
        }
        std::random_device rd;

        unsigned long long seed = rd();

        std::mt19937 gen(seed);

        std::uniform_int_distribution<int> dist(0, 2);

        int itemNum = dist(gen) % 2;

        switch (dist(gen))
        {
        case 0:
            player::UseAttack(&global::player::enemy[global::player::current_enemy], &global::player::player);
            break;
        case 1:
            if (global::player::enemy[global::player::current_enemy].JOB == WARRIOR && global::player::enemy[global::player::current_enemy].skillCount < 3)
            {
                player::UseSkill(&global::player::enemy[global::player::current_enemy], &global::player::player, 1);
                global::player::isUseSkill = true;
            }
            else if (global::player::enemy[global::player::current_enemy].skillCount < 3)
            {
                player::UseSkill(&global::player::enemy[global::player::current_enemy], &global::player::player, (dist(gen) % 2));
                global::player::isUseSkill = true;
            }
            else
                player::UseAttack(&global::player::enemy[global::player::current_enemy], &global::player::player);
            break;
        case 2:
            if (global::player::enemy[global::player::current_enemy].item[itemNum].itemcount > 0)
                player::UseItem(&global::player::enemy[global::player::current_enemy], itemNum);
            break;
        default:
            break;
        }

        global::player::isEnemyTurn = false;
        global::player::isPlayerTurn = true;
    }

    if (global::player::isUseSkill)
    {
        return;
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
    

    StartGame();
    

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

