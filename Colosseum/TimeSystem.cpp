#include "TimeSystem.h"
//using namespace Time; // 모든 타임 네임스페이스 변수 Time:: 자동 대입




void time::InitTime()
{
	currentTime = previousTime = GetTickCount64(); // 현재 시각 및 세이브 시각을 OS틱으로 담아둠
}
void time::UpdateTime()
{
	previousTime = currentTime; // 세이브 시각 갱신
	currentTime = GetTickCount64(); // 현재 시각 갱신

	deltaTime = currentTime - previousTime; // 델타타임 구하기 (현재 - 세이브 시각)
}
ULONGLONG time::GetDeltaTime()
{
	return deltaTime;
}
COORD prePlayerPos; // 기존 플레이어 위치
COORD curPlayerPos; // 현재 플레이어 위치

SMALL_RECT consoleScreenSize;
SMALL_RECT playerMovableRect = { 5, 5, 30, 30 }; // 플레이어가 이동이 가능한 영역
const int playerMoveSpeed = 200;
