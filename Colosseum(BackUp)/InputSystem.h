#pragma once
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

		const int MAX_KEY = 3;

		void Set(const int keyIdx, bool bOn);

		bool IsEscapeCmdOn();
		bool IsLeftCmdOn();
		bool IsRightCmdOn();

		void UpdateInput();
	};
};