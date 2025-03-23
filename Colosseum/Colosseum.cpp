#include <iostream>
#include"TimeSystem.h"
#include"InputSystem.h"
//#include"RenderSystem.h"
#include"GameSystem.h"
#include"Player.h"


int main()
{
    global::time::InitTime(); // 틱 받아오기

    global::game::StartGame();

    //while (IsGameRun())
    //{
    //    UpdateTime();

    //    ProcessInput();
    //    FixeUpdate();

    //    Update();
    //    Render();
    //}

    //EndGame();

    return 0;
}