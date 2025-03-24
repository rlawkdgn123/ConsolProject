#pragma once
#include <Windows.h>
#include "TimeSystem.h"
#include "Define.h"
namespace global
{

    COORD prePlayerPos; // 기존 플레이어 위치
    COORD curPlayerPos; // 현재 플레이어 위치

    COORD enemyWorldBasis = { 10, 2 };

    SMALL_RECT playerMovableRect = { 5, 5, 30, 30 }; // @SEE StartGame()

    const int playerMoveSpeed = 200; // 플레이어 이동 속도

    bool introFlag = false; // 인트로 화면 출력 여부
    int stage = 0;

    bool cussorTP = true; // 커서 순간이동(T) / 연속이동(F) 여부

    // 노가다로-0- 적을 만들어 봅시다.
    const int ENEMY_CNT = 10;
    struct Enemy
    {
        COORD localPos;
        char  character;
    };

    Enemy consoleEnemy[ENEMY_CNT];
};