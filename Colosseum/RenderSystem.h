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

	void ChoiceDraw(int x, int y, const char* pStr, bool highLight);

	void DrawGames(int num, int* menuFlag);
	//char* OpenText(char* fileName, int fileHeight, int fileWidth);
	//char* OpenText(const char* fileName, int fileHeight, int fileWidth);
	wchar_t* OpenText(const char* fileName, int fileHeight, int fileWidth);
	wchar_t* EncodeMap(wchar_t* pMap);

	SMALL_RECT GetPlayerMovableRect();
	void setConsoleSize(int width, int height);
	void DrawBorder();
	void RenderTitle(int* choiceNum);
};