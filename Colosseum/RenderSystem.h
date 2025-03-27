#pragma once
#include <Windows.h>
#include "Player.h"

namespace render
{
	void InitScreen();
	void ScreenRelease();

	void ScreenFlipping();
	void PrintScreen(int x, int y, const char* string);
	void PrintScreen(int x, int y, char* string);
	void ScreenClear();
	void SetColor(unsigned short color);

	//  함수 오버로딩에 대해 알아 봅시다.
	void ScreenDraw(int x, int y, const char c);
	void ScreenDraw(int x, int y, const char* pStr);

	void ChoiceDraw(int x, int y, const char* pStr, bool highLight);

	void DrawGameText(int* menuFlag, COORD* curPlayerPos, int* curIndex, int* curEnemy, PLAYER* player, PLAYER* enemy);
	void DrawStateText(COORD* curPlayerPos, PLAYER* player, PLAYER* enemy, bool* UseAttack, bool* UseSkill, bool* UseItem);
	wchar_t* EncodeMap(wchar_t* pMap);
	void OpenTextAndWrite(int x, int y, const char* fileName);
	void OpenTextAndWriteAnim(int x, int y, const char* dirPath);

	SMALL_RECT GetPlayerMovableRect();
	void setConsoleSize(int width, int height);
	void DrawBorder();
	void RenderTitle(int* choiceNum, COORD* curPlayerPos);
	void RenderHeroChoice(int* choiceNum, COORD* curPlayerPos);
	void RenderMain(int* choiceNum, COORD* curPlayerPos, PLAYER* enemy);
	void RenderBattle(int* choiceNum, COORD* curPlayerPos);
	void RenderBattle_Skill(int* choiceNum, COORD* curPlayerPos, PLAYER* player);
	void RenderBattle_Item(int* choiceNum, COORD* curPlayerPos, PLAYER* player);
	void RenderBattle_End(int* choiceNum, COORD* curPlayerPos);
	void RenderEnd(int* choiceNum, COORD* curPlayerPos);
};