#pragma once
#include <Windows.h>

namespace render
{
	void InitScreen();
	void ScreenRelease();

	void ScreenFlipping();
	void ScreenClear();

	//  �Լ� �����ε��� ���� �˾� ���ô�.
	void ScreenDraw(int x, int y, const char c);
	void ScreenDraw(int x, int y, const char* pStr);


	SMALL_RECT GetPlayerMovableRect();

	void DrawBorder();
};