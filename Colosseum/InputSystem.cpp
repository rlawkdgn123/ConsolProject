#include <windows.h>
#include <cassert>
#include "InputSystem.h"


namespace global
{
	namespace input
	{
		bool inputKeyTable[MAX_KEY]; // ���ǵ� Ű ���� ó�� �մϴ�.
		bool inputLockA = false;
		bool inputLockD = false;
		bool inputLockW = false;
		bool inputLockS = false;
		bool inputLockSPACE = false;
		// Helper Functions ..
		void Set(const int keyIdx, bool bOn)
		{

			assert(keyIdx >= 0 && keyIdx < MAX_KEY); // ��� �ڵ带 �ۼ��ϴ� �Ϳ� �ͼ������� �մϴ�.  �迭�� �ٷ� ���� �ε����� �����ؾ� �մϴ�!!

			inputKeyTable[keyIdx] = bOn;			// bool Ÿ���� Ȱ�� �սô�.
		}

		bool IsEscapeCmdOn() { return inputKeyTable[ESCAPE_KEY_INDEX]; }
		bool IsLeftCmdOn() { return inputKeyTable[USER_CMD_LEFT];  }
		bool IsRightCmdOn() { return inputKeyTable[USER_CMD_RIGHT]; }
		bool IsUpCmdOn() { return inputKeyTable[USER_CMD_LEFT]; }
		bool IsDownCmdOn() { return inputKeyTable[USER_CMD_RIGHT]; }
		bool IsSpaceCmdOn() { return inputKeyTable[USER_CMD_SPACE]; }
		void InputState() { UpdateInput(); }
		// �Է� Ű���� ���� �˾� ���ô�. ��ŷ�� �ͺ�ŷ�� ���ؼ��� �����ϰ� ����� �մϴ�!!
		// * ��Ʈ ���꿡 ���� �н��մϴ�. & �� && �� �޶��. 
		// &, * �����ڰ� �ƶ��� ���� �ٸ��� �ؼ��Ǵ� �Ϳ� ���� �ؾ� �մϴ�. 
		void UpdateInput()
		{
			if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			{
				global::input::Set(ESCAPE_KEY_INDEX, true);
			}
			if (((GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState(VK_LEFT) & 0x8000)) && !inputLockA) //���� 'A' �� ȭ��ǥ
			{
				inputLockA = true;
				global::input::Set(USER_CMD_LEFT, true);
			}
			if (((GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState(VK_RIGHT) & 0x8000)) && !inputLockD) //������ 'D' �� ȭ��ǥ
			{
				inputLockD = true;
				global::input::Set(USER_CMD_RIGHT, true);
			}
			if (((GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState(VK_UP) & 0x8000)) && !inputLockW) //������ 'W' �� ȭ��ǥ
			{
				inputLockW = true;
				global::input::Set(USER_CMD_RIGHT, true);
			}
			if (((GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState(VK_DOWN) & 0x8000)) && !inputLockS) //������ 'S' �� ȭ��ǥ
			{
				inputLockS = true;
				global::input::Set(USER_CMD_RIGHT, true);
			}
			if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && !inputLockSPACE) { // ���� CHOICE �����̽���
				inputLockSPACE = true;
				global::input::Set(USER_CMD_SPACE, true);
			}


			// @@@@@@@@@@@@@@@@@@@@@@
			if (!GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			{
				global::input::Set(ESCAPE_KEY_INDEX, false);
			}
			if ((!(GetAsyncKeyState('A') & 0x8000) && !(GetAsyncKeyState(VK_LEFT) & 0x8000)) && inputLockA) //���� 'A' �� ȭ��ǥ
			{
				inputLockA = false;
			}
			if ((!(GetAsyncKeyState('D') & 0x8000) && !(GetAsyncKeyState(VK_RIGHT) & 0x8000)) && inputLockD) //������ 'D' �� ȭ��ǥ
			{
				inputLockD = false;
			}
			if ((!(GetAsyncKeyState('W') & 0x8000) && !(GetAsyncKeyState(VK_UP) & 0x8000)) && inputLockW) //������ 'W' �� ȭ��ǥ
			{
				inputLockW = false;
				global::input::Set(USER_CMD_RIGHT, false);
			}
			if ((!(GetAsyncKeyState('S') & 0x8000) && !(GetAsyncKeyState(VK_DOWN) & 0x8000)) && inputLockS) //������ 'S' �� ȭ��ǥ
			{
				inputLockS = false;
				global::input::Set(USER_CMD_RIGHT, false);
			}
			if ((!GetAsyncKeyState(VK_SPACE) & 0x8000) && inputLockSPACE) { // ���� CHOICE �����̽���
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
			if ((GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState(VK_LEFT) & 0x8000)) //���� 'A' �� ȭ��ǥ
			{
				global::input::Set(USER_CMD_LEFT, true);
			}
			if ((GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState(VK_RIGHT) & 0x8000)) //������ 'D' �� ȭ��ǥ
			{
				if (GetAsyncKeyState(VK_SPACE) & 0x8000) { // ���� CHOICE �����̽���
					global::input::Set(USER_CMD_SPACE, true);
				}
			}
		};
	};
}



