#pragma once
#include <Windows.h>
#include "TimeSystem.h"
#include "Define.h"
namespace global
{

    COORD prePlayerPos; // ���� �÷��̾� ��ġ
    COORD curPlayerPos; // ���� �÷��̾� ��ġ

    COORD enemyWorldBasis = { 10, 2 };

    SMALL_RECT playerMovableRect = { 5, 5, 30, 30 }; // @SEE StartGame()

    const int playerMoveSpeed = 200; // �÷��̾� �̵� �ӵ�

    bool introFlag = false; // ��Ʈ�� ȭ�� ��� ����
    int stage = 0;

    bool cussorTP = true; // Ŀ�� �����̵�(T) / �����̵�(F) ����

    // �밡�ٷ�-0- ���� ����� ���ô�.
    const int ENEMY_CNT = 10;
    struct Enemy
    {
        COORD localPos;
        char  character;
    };

    Enemy consoleEnemy[ENEMY_CNT];
};