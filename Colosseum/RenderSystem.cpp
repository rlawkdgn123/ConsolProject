#include"RenderSystem.h"

void render::GotoXY(int x, int y)
{
    COORD Cur = { x, y };

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}
void PrintPlayerPostion()
{
    printf("Player Position (%d, %d)", game::curPlayerPos.X, curPlayerPos.Y);
}
void Render()
{
    render::GotoXY(0, 0); // ������ ī��Ʈ ��� ��ġ
    PrintCountsPerSecond();

    render::GotoXY(50, 0); // �÷��̾� ��ġ ����
    PrintPlayerPostion();

    // ��ġ�� �޶����� ����
    if ((prePlayerPos.X != curPlayerPos.X) || (prePlayerPos.Y != curPlayerPos.Y))
    {
        DrawPlayer(true);
    }
    
}

