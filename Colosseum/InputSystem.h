#pragma once
namespace Input {

	constexpr int ESCAPE_KEY_INDEX = 0;
	constexpr int LEFT_KEY_INDEX = 1;
	constexpr int RIGHT_KEY_INDEX = 2;
	constexpr int UP_KEY_INDEX = 3;
	constexpr int DOWN_KEY_INDEX = 4;

	COORD prePlayerPos; // 기존 플레이어 위치
	COORD curPlayerPos; // 현재 플레이어 위치

	SMALL_RECT consoleScreenSize;									// 조종 가능 화면 크기
	SMALL_RECT playerMovableRect = { 5, 5, 30, 30 };   // 플레이어가 이동이 가능한 영역
	const int playerMoveSpeed = 200;								//플레이어 이동속도

	void UpdateInput();
}
