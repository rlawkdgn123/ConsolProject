#pragma once
#include<windows.system.h>
namespace global {
	namespace game {

		// Screen
		SMALL_RECT consoleScreenSize;

		// Player
		COORD prePlayerPos; // 기존 플레이어 위치
		COORD curPlayerPos; // 현재 플레이어 위치
		SMALL_RECT playerMovableRect = { 5, 5, 30, 30 }; // 플레이어가 이동이 가능한 영역
		const int playerMoveSpeed = 200;




		void StartGame();
		bool IsGameRun();
		void EndGame();
		void GotoXY(int x, int y);

	}
}