#pragma once
#include <Windows.h>
#include "Player.h"

namespace render
{
	void InitScreen();
	void ScreenRelease();

	void ScreenFlipping();
	void PrintScreen(int x, int y, char* string);
	void ScreenClear();
	void SetColor(unsigned short color);

	//  �Լ� �����ε��� ���� �˾� ���ô�.
	void ScreenDraw(int x, int y, const char c);
	void ScreenDraw(int x, int y, const char* pStr);

	void ChoiceDraw(int x, int y, const char* pStr, bool highLight);

	void DrawGames(int* menuFlag, COORD* curPlayerPos, int* curIndex, int* curEnemy, PLAYER* player, PLAYER* enemy);
	//char* OpenText(char* fileName, int fileHeight, int fileWidth);
	//char* OpenText(const char* fileName, int fileHeight, int fileWidth);
	wchar_t* OpenText(const char* fileName, int fileHeight, int fileWidth);
	wchar_t* EncodeMap(wchar_t* pMap);

	SMALL_RECT GetPlayerMovableRect();
	void setConsoleSize(int width, int height);
	void DrawBorder();
	void RenderTitle(int* choiceNum, COORD* curPlayerPos);
	void RenderHeroChoice(int* choiceNum, COORD* curPlayerPos);
	void RenderMain(int* choiceNum, COORD* curPlayerPos, PLAYER* enemy);
};