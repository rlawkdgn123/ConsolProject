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
    render::GotoXY(0, 0); // 프레임 카운트 출력 위치
    PrintCountsPerSecond();

    render::GotoXY(50, 0); // 플레이어 위치 정보
    PrintPlayerPostion();

    // 위치가 달라지면 갱신
    if ((prePlayerPos.X != curPlayerPos.X) || (prePlayerPos.Y != curPlayerPos.Y))
    {
        DrawPlayer(true);
    }
    
}

