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

	//  �Լ� �����ε��� ���� �˾� ���ô�.
	void ScreenDraw(int x, int y, const char c);
	void ScreenDraw(int x, int y, const char* pStr);

	void ChoiceDraw(int x, int y, const char* pStr, bool highlight, int color);

	void DrawGameText(int* menuFlag, COORD* curPlayerPos, int* curIndex, int* curEnemy, PLAYER* player, PLAYER* enemy);
	void DrawStateText(COORD* curPlayerPos, PLAYER* player, PLAYER* enemy, bool* UseAttack, bool* UseSkill, bool* UseItem);
	void DrawHP(PLAYER* player, PLAYER* enemy);
	wchar_t* EncodeMap(wchar_t* pMap);
	void OpenTextAndWrite(int x, int y, const char* fileName);
	void OpenTextAndWrite(int x, int y, const char* fileName, bool isEnemy);
	void OpenTextAndWriteAnim(int xpos, int ypos, const char** fileName, int count, int color, bool isEnemy);

	SMALL_RECT GetPlayerMovableRect();
	void setConsoleSize(int width, int height);
	void DrawBorder();
	void RenderPLAYERS(PLAYER* player, PLAYER* enemy);
	void RenderTitle(COORD* curPlayerPos);
	void RenderHeroChoice(COORD* curPlayerPos);
	void RenderMain(COORD* curPlayerPos, PLAYER* enemy);
	void RenderBattle(COORD* curPlayerPos);
	void RenderBattle_Skill(COORD* curPlayerPos, PLAYER* player);
	void RenderBattle_Item( COORD* curPlayerPos, PLAYER* player);
	void RenderBattle_End(COORD* curPlayerPos);
	void RenderEnd(COORD* curPlayerPos);
	void RenderGameOver(COORD* curPlayerPos);
	void RenderClear(COORD* curPlayerPos);
};