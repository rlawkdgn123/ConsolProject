#pragma once
namespace global
{
	// �Լ��� ����� ���� ���信 ���� ������ ���ô�.
	// �ʿ��� �������̽��� �����Ѵٴ� ���信 ���� ������ ���ô�.
	namespace input
	{
		// ����� ���� �ϴ� ����� ���� �˾� ���ô�. 
		// const Ű�� #define �� ���̸� �˾ƾ� �մϴ�. ��Ʈ! const �� ������ ���� Ű�����̰� #�� ������ ���������Ͽ� ���� �˴ϴ�.
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