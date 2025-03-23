#include "TimeSystem.h"
//using namespace Time; // ��� Ÿ�� ���ӽ����̽� ���� Time:: �ڵ� ����

namespace global
{
	namespace time {

        void InitTime()
        {
            currentTime = previousTime = GetTickCount64();
        }

        void UpdateTime()
        {
            previousTime = currentTime;
            currentTime = GetTickCount64();

            deltaTime = currentTime - previousTime;
        }
	}
}