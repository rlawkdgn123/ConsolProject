#include "TimeSystem.h"
//using namespace Time; // ��� Ÿ�� ���ӽ����̽� ���� Time:: �ڵ� ����


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