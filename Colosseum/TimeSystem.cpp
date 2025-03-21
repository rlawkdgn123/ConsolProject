#include "TimeSystem.h"
//using namespace Time; // ��� Ÿ�� ���ӽ����̽� ���� Time:: �ڵ� ����




void time::InitTime()
{
	currentTime = previousTime = GetTickCount64(); // ���� �ð� �� ���̺� �ð��� OSƽ���� ��Ƶ�
}
void time::UpdateTime()
{
	previousTime = currentTime; // ���̺� �ð� ����
	currentTime = GetTickCount64(); // ���� �ð� ����

	deltaTime = currentTime - previousTime; // ��ŸŸ�� ���ϱ� (���� - ���̺� �ð�)
}
ULONGLONG time::GetDeltaTime()
{
	return deltaTime;
}
COORD prePlayerPos; // ���� �÷��̾� ��ġ
COORD curPlayerPos; // ���� �÷��̾� ��ġ

SMALL_RECT consoleScreenSize;
SMALL_RECT playerMovableRect = { 5, 5, 30, 30 }; // �÷��̾ �̵��� ������ ����
const int playerMoveSpeed = 200;
