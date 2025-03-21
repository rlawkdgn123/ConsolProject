#pragma once
#include <stdio.h>
#include <Windows.h>

namespace Time {
	ULONGLONG previousTime;
	ULONGLONG currentTime;
	ULONGLONG deltaTime;

	int updateCount;
	int fixedUpdateCount;

	void InitTime();
	void UpdateTime();
	ULONGLONG GetDeltaTime();
};
