#pragma once
#include "Define.h"
namespace global
{
	// �Լ��� ����� ���� ���信 ���� ������ ���ô�.
	// �ʿ��� �������̽��� �����Ѵٴ� ���信 ���� ������ ���ô�.
	namespace input
	{
		// ����� ���� �ϴ� ����� ���� �˾� ���ô�. 
		// const Ű�� #define �� ���̸� �˾ƾ� �մϴ�. ��Ʈ! const �� ������ ���� Ű�����̰� #�� ������ ���������Ͽ� ���� �˴ϴ�.
		const int ESCAPE_KEY_INDEX = 0;
		const int USER_CMD_LEFT = 1;
		const int USER_CMD_RIGHT = 2;
		const int USER_CMD_UP = 3;
		const int USER_CMD_DOWN = 4;
		const int USER_CMD_SPACE = 5;

		const int MAX_KEY = 6;

		void Set(const int keyIdx, bool bOn);

		bool IsEscapeCmdOn();
		bool IsLeftCmdOn();
		bool IsRightCmdOn();
		bool IsUpCmdOn();
		bool IsDownCmdOn();
		bool IsSpaceCmdOn();

		void InputState(int* menuFlag); // ��ǲ ���� ������
		void UpdateInput();
		void TitleInput();
		void HeroChoiceInput();
	};
};