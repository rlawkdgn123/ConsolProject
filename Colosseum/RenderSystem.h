#pragma once
#include <Windows.h>
#include "Define.h"
#include "timeSystem.h"
namespace render
{
	void InitScreen();
	void ScreenRelease();

	void ScreenFlipping();
	void ScreenClear();

	//  함수 오버로딩에 대해 알아 봅시다.
	void ScreenDraw(int x, int y, const char c);
	void ScreenDraw(int x, int y, const char* pStr);

	void DrawGames(int num);
	char* OpenText(char* fileName, int fileHeight, int fileWidth);
	char* OpenText(const char* fileName, int fileHeight, int fileWidth);
	SMALL_RECT GetPlayerMovableRect();
	void setConsoleSize(int width, int height);
	void DrawBorder();
	void RenderTitle(int* menuFlag);
};