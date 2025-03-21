#pragma once
#include <Windows.h>
#include "InputSystem.h"
#include "RenderSystem.h"
namespace global
{
    namespace time
    {
        ULONGLONG previousTime;
        ULONGLONG currentTime;
        ULONGLONG deltaTime;

        int updateCount;
        int fixedUpdateCount;

        void InitTime();
        
        void UpdateTime();
        
    }
}

typedef struct {
    char* skillName;
    int skillDamage;
}SKILL;

typedef struct {
    char* itemName;
    int healing = 0;
    int stun = 0;
}ITEM;

typedef struct {
    int hp;
    int attackDamage;
    SKILL skill[4];
}PLAYER;

void StartGame();
void Clamp(short& n, short min, short max); // 레퍼런스 타입에 대해 배워 봅시다.
void DrawPlayer();
void DrawEnemy();
void UpdateCussorPosition();
void UpdatePlayer();
void UpdateEnemy();
void EndGame();
void ProcessInput();
void PrintCountsPerSecond();
void PrintPlayerPostion();
void Render();
bool IsGameRun();
void FixeUpdate();
void Update();
void PrintPlayerPostion();
void PrintCountsPerSecond();