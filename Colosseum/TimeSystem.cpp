#include "TimeSystem.h"
//using namespace Time; // 모든 타임 네임스페이스 변수 Time:: 자동 대입


void global::time::InitTime()
{
    currentTime = previousTime = GetTickCount64();
}

void global::time::UpdateTime()
{
    previousTime = currentTime;
    currentTime = GetTickCount64();

    deltaTime = currentTime - previousTime;
}