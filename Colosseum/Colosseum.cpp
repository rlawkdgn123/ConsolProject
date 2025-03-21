#pragma once;
#include <stdio.h>
#include <windows.h>
#include <random>
#include <iostream>

#include "InputSystem.h" // 기능 별로 모듈화를 한다는 개념에 대해 생각해 봅시다!
#include "GameSystem.cpp"

int main()
{
    global::time::InitTime();

    StartGame();

    while (IsGameRun())
    {
        global::time::UpdateTime();

        ProcessInput();
        FixeUpdate();

        Update();
        Render();
    }

    EndGame();

    return 0;
}