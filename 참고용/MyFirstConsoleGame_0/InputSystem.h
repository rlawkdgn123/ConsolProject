#pragma once
namespace global
{
	// 함수의 선언과 정의 개념에 대해 생각해 봅시다.
	// 필요한 인터페이스만 노출한다는 개념에 대해 생각해 봅시다.
	namespace input
	{
		// 상수를 정의 하는 방법에 대해 알아 봅시다. 
		// const 키와 #define 의 차이를 알아야 합니다. 힌트! const 는 변수에 붙은 키워드이고 #이 붙으면 프리컴파일에 결정 됩니다.
		constexpr int ESCAPE_KEY_INDEX = 0;
		constexpr int LEFT_KEY_INDEX = 1;
		constexpr int RIGHT_KEY_INDEX = 2;
		constexpr int UP_KEY_INDEX = 3;
		constexpr int DOWN_KEY_INDEX = 4;

		const int MAX_KEY = 5;

		void Set(const int keyIdx, bool bOn);

		bool IsLeftKeyOn();
		bool IsRightKeyOn();
		bool IsUpKeyOn();
		bool IsDownKeyOn();

		void UpdateInput();
	};
};