
#include <stdio.h>
#include <windows.h>
#include "InputSystem.h" // 기능 별로 모듈화를 한다는 개념에 대해 생각해 봅시다!

namespace global
{
    namespace time
    {
        ULONGLONG previousTime;
        ULONGLONG currentTime;
        ULONGLONG deltaTime;

        int updateCount;
        int fixedUpdateCount;
  
        void InitTime()
        {
           currentTime = previousTime = GetTickCount64();
        }

        void UpdateTime()
        {
            previousTime = currentTime;
            currentTime = GetTickCount64();

            deltaTime = currentTime - previousTime;
        }

        ULONGLONG GetDeltaTime()
        {
            return deltaTime;
        }
    };

    COORD prePlayerPos; // 기존 플레이어 위치
    COORD curPlayerPos; // 현재 플레이어 위치
    
    SMALL_RECT consoleScreenSize;
    SMALL_RECT playerMovableRect = { 5, 5, 30, 30 }; // 플레이어가 이동이 가능한 영역

    const int playerMoveSpeed = 200;

};

void Clamp(short& n, short min, short max) // 레퍼런스 타입에 대해 배워 봅시다.
{
    if (n < min) n = min;
    if (n > max) n = max;
}

void GotoXY(int x, int y)
{
    COORD Cur = { x, y };

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void DrawPlayer(bool bClear)
{
    if (bClear)
    {
        // 플레이어 기존 위치
        GotoXY(global::prePlayerPos.X, global::prePlayerPos.Y);
        putchar(' ');
    }

    // 플레이어 현재 위치
    GotoXY(global::curPlayerPos.X, global::curPlayerPos.Y);
    putchar('@');
}

void UpdatePlayerPosition()
{
    global::prePlayerPos = global::curPlayerPos; // 현재 위치 경신 전에 일단, 저장. 구조체를 쓰면 이런게 편한겁니다. :)

    if (global::input::IsUpKeyOn())
    {
        global::input::Set(global::input::UP_KEY_INDEX, false);
        
        global::curPlayerPos.Y--;
        Clamp(global::curPlayerPos.Y, global::playerMovableRect.Top, global::playerMovableRect.Bottom); // 이동 범위를 제한 합니다. 
    }

    if (global::input::IsDownKeyOn())
    {
        global::input::Set(global::input::DOWN_KEY_INDEX, false);

        global::curPlayerPos.Y++;
        Clamp(global::curPlayerPos.Y, global::playerMovableRect.Top, global::playerMovableRect.Bottom);
    }

    if (global::input::IsLeftKeyOn())
    {
        global::input::Set(global::input::LEFT_KEY_INDEX, false);

        global::curPlayerPos.X--;
        Clamp(global::curPlayerPos.X, global::playerMovableRect.Left, global::playerMovableRect.Right);
    }

    if (global::input::IsRightKeyOn())
    {
        global::input::Set(global::input::RIGHT_KEY_INDEX, false);

        global::curPlayerPos.X++;
        Clamp(global::curPlayerPos.X, global::playerMovableRect.Left, global::playerMovableRect.Right);
    }
}

void UpdatePlayer()
{
    // 키 입력과 화면 출력과 게임 로직 갱신을 분리해서 생각해 봅시다.
    static ULONGLONG elapsedTime;

    elapsedTime += global::time::GetDeltaTime();

    while (elapsedTime >= global::playerMoveSpeed)
    {
        UpdatePlayerPosition();

        elapsedTime -= global::playerMoveSpeed;
    }
}

void DrawMovableRect()
{
    // 위쪽 라인. Y 값이 고정 된다.
    for (int x = global::playerMovableRect.Left - 1; x < global::playerMovableRect.Right + 1; x++)
    {
        GotoXY(x, global::playerMovableRect.Top - 1);
        putchar('#');
    }

    // 아래쪽 라인. Y 값이 고정 된다.
    for (int x = global::playerMovableRect.Left - 1; x < global::playerMovableRect.Right + 1; x++)
    {
        GotoXY(x, global::playerMovableRect.Bottom + 1);
        putchar('#');
    }

    // 왼쪽 라인, X 값이 고정 된다.
    for (int y = global::playerMovableRect.Top - 1; y < global::playerMovableRect.Bottom + 1; y++)
    {
        GotoXY(global::playerMovableRect.Left - 1, y);
        putchar('#');
    }

    // 오른쪽 라인, X 값이 고정 된다.
    for (int y = global::playerMovableRect.Top - 1; y < global::playerMovableRect.Bottom + 1; y++)
    {
        GotoXY(global::playerMovableRect.Right + 1, y);
        putchar('#');
    }
}

void StartGame()
{
    // 깜박이는 커서를 좀 진정 시키자.
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.bVisible = 0; // 커서를 보일지 말지 결정(0이면 안보임, 0제외 숫자 값이면 보임)
    cursorInfo.dwSize = 1; // 커서의 크기를 결정 (1~100 사이만 가능)
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    // 콘솔창 크기를 가져 오고
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    
    global::consoleScreenSize.Left = csbi.srWindow.Left;
    global::consoleScreenSize.Right = csbi.srWindow.Right;
    global::consoleScreenSize.Bottom = csbi.srWindow.Bottom;
    global::consoleScreenSize.Top = csbi.srWindow.Top;

    // 플레이어가 이동 가능한 영역을 설정 하자. 
    global::playerMovableRect.Left = global::consoleScreenSize.Left + 2;
    global::playerMovableRect.Right = global::consoleScreenSize.Right - 2;
    global::playerMovableRect.Bottom = global::consoleScreenSize.Bottom - 2;
    global::playerMovableRect.Top = global::consoleScreenSize.Top + 2;

    DrawMovableRect(); // 벽을 그려 놓자!

    // 플레이어 시작 위치 설정
    global::prePlayerPos.X = 10;
    global::prePlayerPos.Y = 10;
    global::curPlayerPos.X = 10;
    global::curPlayerPos.Y = 10;

    DrawPlayer(false);
}

void EndGame()
{

}

void ProcessInput()
{
    global::input::UpdateInput();
}

void PrintCountsPerSecond();
void PrintPlayerPostion();

void Render()
{
    GotoXY(0, 0); // 프레임 카운트 출력 위치
    PrintCountsPerSecond();

    GotoXY(50, 0); // 플레이어 위치 정보
    PrintPlayerPostion();

    // 위치가 달라지면 갱신
    if ((global::prePlayerPos.X != global::curPlayerPos.X) || (global::prePlayerPos.Y != global::curPlayerPos.Y))
    {
        DrawPlayer(true);
    }

}

bool IsGameRun()
{
    return true;
}

void FixeUpdate()
{
    static ULONGLONG elapsedTime;

    elapsedTime += global::time::GetDeltaTime();

    while (elapsedTime >= 20) //0.02초
    {
        global::time::fixedUpdateCount += 1;

        elapsedTime -= 20;
    }
}

void Update()
{
    global::time::updateCount += 1;

    UpdatePlayer();

}

void PrintPlayerPostion()
{
    printf("Player Position (%d, %d)", global::curPlayerPos.X, global::curPlayerPos.Y);
}

void PrintCountsPerSecond()
{
    static ULONGLONG elapsedTime;

    elapsedTime += global::time::GetDeltaTime();
    if (elapsedTime >= 1000)
    {
        printf("updateCount : %d \t fixedUpdateCount : %d \n", 
            global::time::updateCount, global::time::fixedUpdateCount);


        elapsedTime = 0;
        global::time::updateCount = 0;
        global::time::fixedUpdateCount = 0;
    }
}

int main()
{
    global::time::InitTime();

    StartGame();

    while (IsGameRun())
    {
        global::time::UpdateTime();

        ProcessInput();
        FixeUpdate();
        
        Update();
        Render();
    }

    EndGame();

    return 0;
}

