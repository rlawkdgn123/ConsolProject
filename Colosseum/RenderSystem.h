#pragma once
#include <Windows.h>

namespace render
{
	void InitScreen();
	void ScreenRelease();

	void ScreenFlipping();
	void ScreenClear();

	//  함수 오버로딩에 대해 알아 봅시다.
	void ScreenDraw(int x, int y, const char c);
	void ScreenDraw(int x, int y, const char* pStr);


	SMALL_RECT GetPlayerMovableRect();

	void DrawBorder();
};