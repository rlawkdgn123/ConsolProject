#pragma once;
#include <stdio.h>
#include <random>
#include <iostream>
#include "GameSystem.h"

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