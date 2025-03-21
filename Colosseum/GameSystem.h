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

void StartGame();
void Clamp(short& n, short min, short max); // ���۷��� Ÿ�Կ� ���� ��� ���ô�.
void DrawPlayer();
void DrawEnemy();
void UpdatePlayerPosition();
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