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