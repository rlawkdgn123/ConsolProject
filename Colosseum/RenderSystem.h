#pragma once
#include <stdio.h>
#include <windows.h>

namespace global {
	namespace render {
        bool bScreenIndex;
        HANDLE hScreen[2];

        SMALL_RECT consoleScreenSize;
        INT consoleScreenX;
        INT consoleScreenY;

        SMALL_RECT updateScreenSize;
        INT updateScreenX;
        INT updateScreenY;

        HANDLE GetScreenHandle();
        void DrawPlayer();
        void ScreenDraw(int x, int y, const char c);
        void InitScreen();
        SMALL_RECT GetPlayerMovableRect();
        void GotoXY(int x, int y);
    }
}
