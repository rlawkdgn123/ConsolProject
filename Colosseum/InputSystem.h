#pragma once
namespace Input {

	constexpr int ESCAPE_KEY_INDEX = 0;
	constexpr int LEFT_KEY_INDEX = 1;
	constexpr int RIGHT_KEY_INDEX = 2;
	constexpr int UP_KEY_INDEX = 3;
	constexpr int DOWN_KEY_INDEX = 4;

	COORD prePlayerPos; // ���� �÷��̾� ��ġ
	COORD curPlayerPos; // ���� �÷��̾� ��ġ

	SMALL_RECT consoleScreenSize;									// ���� ���� ȭ�� ũ��
	SMALL_RECT playerMovableRect = { 5, 5, 30, 30 };   // �÷��̾ �̵��� ������ ����
	const int playerMoveSpeed = 200;								//�÷��̾� �̵��ӵ�

	void UpdateInput();
}
