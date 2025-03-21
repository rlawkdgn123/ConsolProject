#include "TimeSystem.h"
using namespace Time; // 모든 타임 네임스페이스 변수 Time:: 자동 대입
void InitTime()
{
	currentTime = previousTime = GetTickCount64(); // 현재 시각 및 세이브 시각을 OS틱으로 담아둠
}
void UpdateTime()
{
	previousTime = currentTime; // 세이브 시각 갱신
	currentTime = GetTickCount64(); // 현재 시각 갱신

	deltaTime = currentTime - previousTime; // 델타타임 구하기 (현재 - 세이브 시각)
}
