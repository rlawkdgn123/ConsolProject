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

	void ChoiceDraw(int x, int y, const wchar_t* pStr, bool highLight);

	void DrawGames(int num, int* menuFlag);
	char* OpenText(char* fileName, int fileHeight, int fileWidth);
	char* OpenText(const char* fileName, int fileHeight, int fileWidth);
	SMALL_RECT GetPlayerMovableRect();
	void setConsoleSize(int width, int height);
	void DrawBorder();
	void RenderTitle(int* choiceNum);
};