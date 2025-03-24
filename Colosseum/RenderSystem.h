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

	//  �Լ� �����ε��� ���� �˾� ���ô�.
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