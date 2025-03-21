#pragma once
#include <Windows.h>
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
        void Clamp(short& n, short min, short max); // 레퍼런스 타입에 대해 배워 봅시다.
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
    }
}