#include "TimeSystem.h"
using namespace Time; // ��� Ÿ�� ���ӽ����̽� ���� Time:: �ڵ� ����
void InitTime()
{
	currentTime = previousTime = GetTickCount64(); // ���� �ð� �� ���̺� �ð��� OSƽ���� ��Ƶ�
}
void UpdateTime()
{
	previousTime = currentTime; // ���̺� �ð� ����
	currentTime = GetTickCount64(); // ���� �ð� ����

	deltaTime = currentTime - previousTime; // ��ŸŸ�� ���ϱ� (���� - ���̺� �ð�)
}
