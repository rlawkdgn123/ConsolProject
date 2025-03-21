#include <iostream>
#include"TimeSystem.h"
#include"InputSystem.h"
#include"RenderSystem.h"
#include"Player.h"

int main()
{
    time::InitTime();

    StartGame();

    while (IsGameRun())
    {
        UpdateTime();

        ProcessInput();
        FixeUpdate();

        Update();
        Render();
    }

    EndGame();

    return 0;
}