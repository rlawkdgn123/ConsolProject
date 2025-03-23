#pragma once
#include<windows.system.h>
namespace global {
	namespace game {

		// Screen
		SMALL_RECT consoleScreenSize;

		// Player
		COORD prePlayerPos; // ���� �÷��̾� ��ġ
		COORD curPlayerPos; // ���� �÷��̾� ��ġ
		SMALL_RECT playerMovableRect = { 5, 5, 30, 30 }; // �÷��̾ �̵��� ������ ����
		const int playerMoveSpeed = 200;




		void StartGame();
		bool IsGameRun();
		void EndGame();
		void GotoXY(int x, int y);

	}
}