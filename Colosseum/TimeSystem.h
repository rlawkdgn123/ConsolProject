#pragma once
#include <Windows.h>
namespace global
{
    namespace time
    {
        ULONGLONG previousTime;
        ULONGLONG currentTime;
        ULONGLONG deltaTime;

        int updateCount;
        int fixedUpdateCount;

        void InitTime()
        {
            currentTime = previousTime = GetTickCount64(); // ƽ ī��Ʈ �񱳷� �ð� ���
        }
        void UpdateTime()
        {
            previousTime = currentTime;
            currentTime = GetTickCount64();

            deltaTime = currentTime - previousTime; // ��Ÿ Ÿ�� ���ϱ�
        }
        ULONGLONG GetDeltaTime()
        {
            return deltaTime;
        }
    };
};