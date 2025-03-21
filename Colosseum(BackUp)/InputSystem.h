#pragma once
namespace global
{
	// 함수의 선언과 정의 개념에 대해 생각해 봅시다.
	// 필요한 인터페이스만 노출한다는 개념에 대해 생각해 봅시다.
	namespace input
	{
		// 상수를 정의 하는 방법에 대해 알아 봅시다. 
		// const 키와 #define 의 차이를 알아야 합니다. 힌트! const 는 변수에 붙은 키워드이고 #이 붙으면 프리컴파일에 결정 됩니다.
		const int ESCAPE_KEY_INDEX = 0;
		const int USER_CMD_LEFT = 1;
		const int USER_CMD_RIGHT = 2;

		const int MAX_KEY = 3;

		void Set(const int keyIdx, bool bOn);

		bool IsEscapeCmdOn();
		bool IsLeftCmdOn();
		bool IsRightCmdOn();

		void UpdateInput();
	};
};