#include <windows.h>
#include <cassert>
#include "InputSystem.h"


namespace global
{
	namespace input
	{
		bool inputKeyTable[MAX_KEY]; // 정의된 키 값을 처리 합니다.
		bool inputLockA = false;
		bool inputLockD = false;
		bool inputLockW = false;
		bool inputLockS = false;
		bool inputLockSPACE = false;
		// Helper Functions ..
		void Set(const int keyIdx, bool bOn)
		{

			assert(keyIdx >= 0 && keyIdx < MAX_KEY); // 방어 코드를 작성하는 것에 익숙해져야 합니다.  배열을 다룰 때는 인덱스를 조심해야 합니다!!

			inputKeyTable[keyIdx] = bOn;			// bool 타입을 활용 합시다.
		}

		bool IsEscapeCmdOn() { return inputKeyTable[ESCAPE_KEY_INDEX]; }
		bool IsLeftCmdOn() { return inputKeyTable[USER_CMD_LEFT];  }
		bool IsRightCmdOn() { return inputKeyTable[USER_CMD_RIGHT]; }
		bool IsUpCmdOn() { return inputKeyTable[USER_CMD_LEFT]; }
		bool IsDownCmdOn() { return inputKeyTable[USER_CMD_RIGHT]; }
		bool IsSpaceCmdOn() { return inputKeyTable[USER_CMD_SPACE]; }
		void InputState() { UpdateInput(); }
		// 입력 키값에 대해 알아 봅시다. 블러킹과 넌블러킹에 대해서만 이해하고 가기로 합니다!!
		// * 비트 연산에 대해 학습합니다. & 와 && 은 달라요. 
		// &, * 연산자가 맥락에 따라 다르게 해석되는 것에 주의 해야 합니다. 
		void UpdateInput()
		{
			if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			{
				global::input::Set(ESCAPE_KEY_INDEX, true);
			}
			if (((GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState(VK_LEFT) & 0x8000)) && !inputLockA) //왼쪽 'A' 및 화살표
			{
				inputLockA = true;
				global::input::Set(USER_CMD_LEFT, true);
			}
			if (((GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState(VK_RIGHT) & 0x8000)) && !inputLockD) //오른쪽 'D' 및 화살표
			{
				inputLockD = true;
				global::input::Set(USER_CMD_RIGHT, true);
			}
			if (((GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState(VK_UP) & 0x8000)) && !inputLockW) //오른쪽 'W' 및 화살표
			{
				inputLockW = true;
				global::input::Set(USER_CMD_RIGHT, true);
			}
			if (((GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState(VK_DOWN) & 0x8000)) && !inputLockS) //오른쪽 'S' 및 화살표
			{
				inputLockS = true;
				global::input::Set(USER_CMD_RIGHT, true);
			}
			if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && !inputLockSPACE) { // 선택 CHOICE 스페이스바
				inputLockSPACE = true;
				global::input::Set(USER_CMD_SPACE, true);
			}


			// @@@@@@@@@@@@@@@@@@@@@@
			if (!GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			{
				global::input::Set(ESCAPE_KEY_INDEX, false);
			}
			if ((!(GetAsyncKeyState('A') & 0x8000) && !(GetAsyncKeyState(VK_LEFT) & 0x8000)) && inputLockA) //왼쪽 'A' 및 화살표
			{
				inputLockA = false;
			}
			if ((!(GetAsyncKeyState('D') & 0x8000) && !(GetAsyncKeyState(VK_RIGHT) & 0x8000)) && inputLockD) //오른쪽 'D' 및 화살표
			{
				inputLockD = false;
			}
			if ((!(GetAsyncKeyState('W') & 0x8000) && !(GetAsyncKeyState(VK_UP) & 0x8000)) && inputLockW) //오른쪽 'W' 및 화살표
			{
				inputLockW = false;
				global::input::Set(USER_CMD_RIGHT, false);
			}
			if ((!(GetAsyncKeyState('S') & 0x8000) && !(GetAsyncKeyState(VK_DOWN) & 0x8000)) && inputLockS) //오른쪽 'S' 및 화살표
			{
				inputLockS = false;
				global::input::Set(USER_CMD_RIGHT, false);
			}
			if ((!GetAsyncKeyState(VK_SPACE) & 0x8000) && inputLockSPACE) { // 선택 CHOICE 스페이스바
				inputLockSPACE = false;
				global::input::Set(USER_CMD_SPACE, false);
			}
		}
		void TitleInput()
		{
			if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			{
				global::input::Set(ESCAPE_KEY_INDEX, true);
			}
			if ((GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState(VK_LEFT) & 0x8000)) //왼쪽 'A' 및 화살표
			{
				global::input::Set(USER_CMD_LEFT, true);
			}
			if ((GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState(VK_RIGHT) & 0x8000)) //오른쪽 'D' 및 화살표
			{
				if (GetAsyncKeyState(VK_SPACE) & 0x8000) { // 선택 CHOICE 스페이스바
					global::input::Set(USER_CMD_SPACE, true);
				}
			}
		};
	};
}



