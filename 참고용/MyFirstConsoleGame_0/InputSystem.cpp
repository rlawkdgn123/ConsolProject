#include <windows.h>
#include <cassert>
#include "InputSystem.h"


namespace global
{
	namespace input
	{
		bool inputKeyTable[MAX_KEY]; // ���ǵ� 5���� Ű ���� ó�� �մϴ�.

		// Helper Functions ..
		void Set(const int keyIdx, bool bOn) 
		{	
			assert(keyIdx >= 0 && keyIdx < MAX_KEY); // ��� �ڵ带 �ۼ��ϴ� �Ϳ� �ͼ������� �մϴ�.  �迭�� �ٷ� ���� �ε����� �����ؾ� �մϴ�!!

			inputKeyTable[keyIdx] = bOn;			// bool Ÿ���� Ȱ�� �սô�.
		}

		bool IsLeftKeyOn()
		{
			return inputKeyTable[LEFT_KEY_INDEX];
		}

		bool IsRightKeyOn()
		{
			return inputKeyTable[RIGHT_KEY_INDEX];
		}

		bool IsUpKeyOn()
		{
			return inputKeyTable[UP_KEY_INDEX];
		}

		bool IsDownKeyOn()
		{
			return inputKeyTable[DOWN_KEY_INDEX];
		}

		// �Է� Ű���� ���� �˾� ���ô�. ��ŷ�� �ͺ�ŷ�� ���ؼ��� �����ϰ� ����� �մϴ�!!
		// * ��Ʈ ���꿡 ���� �н��մϴ�. & �� && �� �޶��. 
		// &, * �����ڰ� �ƶ��� ���� �ٸ��� �ؼ��Ǵ� �Ϳ� ���� �ؾ� �մϴ�. 
		void UpdateInput()
		{
			if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			{
				global::input::Set(ESCAPE_KEY_INDEX, true);
			}
			
			if (GetAsyncKeyState(VK_LEFT) & 0x8000) //����
			{
				global::input::Set(LEFT_KEY_INDEX, true);
			}
			
			if (GetAsyncKeyState(VK_RIGHT) & 0x8000) //������
			{
				global::input::Set(RIGHT_KEY_INDEX, true);
			}
			
			if (GetAsyncKeyState(VK_UP) & 0x8000) //��
			{
				global::input::Set(UP_KEY_INDEX, true);
			}
			
			if (GetAsyncKeyState(VK_DOWN) & 0x8000) //�Ʒ�
			{
				global::input::Set(DOWN_KEY_INDEX, true);
			}
		}
	};
};



