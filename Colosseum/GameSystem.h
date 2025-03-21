#pragma once;
#include<windows.system.h>

namespace game {
	SMALL_RECT consoleScreenSize;
	SMALL_RECT playerMovableRect = { 5, 5, 30, 30 }; // 플레이어가 이동이 가능한 영역
	const int playerMoveSpeed = 200;


	void StartGame();
	bool IsGameRun();
	void EndGame();
	void GotoXY(int x, int y);
	
}