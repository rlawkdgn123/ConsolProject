#include <stdio.h>

#include "renderSystem.h"
#include "Define.h"

#include "InputSystem.h"
using namespace render;
using namespace global::input;
namespace render
{

    bool bScreenIndex;
    HANDLE hScreen[2];

    SMALL_RECT consoleScreenSize;
    INT consoleScreenX;
    INT consoleScreenY;

    SMALL_RECT updateScreenSize;
    INT updateScreenX;
    INT updateScreenY;

    SMALL_RECT GetPlayerMovableRect()
    {
        return updateScreenSize;
    }

    HANDLE GetScreenHandle()
    {
        int index = (bScreenIndex ? 1 : 0);

        return hScreen[index];
    }

    void GotoXY(int x, int y)
    {
        COORD Cur = { x, y };

        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
    }

    void setConsoleSize(int width, int height) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        // 콘솔 화면 버퍼 크기 설정
        COORD bufferSize = { (short)width, (short)height };
        SetConsoleScreenBufferSize(hConsole, bufferSize);

        // 콘솔 창 크기 설정
        SMALL_RECT windowSize = { 0, 0, (short)(width - 1), (short)(height - 1) };
        SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

    }
    void InitScreen()
    {
        setConsoleSize(SCREEN_WIDTH, SCREEN_HEIGHT);

        // 화면 버퍼 2개를 만든다.
        hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

        // 깜박이는 커서를 좀 진정 시키자.
        CONSOLE_CURSOR_INFO cursorInfo = { 0 };
        cursorInfo.bVisible = 0; // 커서를 보일지 말지 결정(0이면 안보임, 0제외 숫자 값이면 보임)
        cursorInfo.dwSize = 1; // 커서의 크기를 결정 (1~100 사이만 가능)
        SetConsoleCursorInfo(hScreen[0], &cursorInfo);
        SetConsoleCursorInfo(hScreen[1], &cursorInfo);


        // 콘솔창 크기를 가져 오자.
        // 지정할 수도 있는데 학습 용으로 그냥, 가져오는 예제로 작성.
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

        /*consoleScreenSize.Left = csbi.srWindow.Left;
        consoleScreenSize.Right = csbi.srWindow.Right;
        consoleScreenSize.Bottom = csbi.srWindow.Bottom;
        consoleScreenSize.Top = csbi.srWindow.Top;*/
        consoleScreenSize.Left = csbi.srWindow.Left;
        consoleScreenSize.Right = csbi.srWindow.Right;
        consoleScreenSize.Bottom = csbi.srWindow.Bottom;
        consoleScreenSize.Top = csbi.srWindow.Top;

        consoleScreenX = csbi.dwSize.X;
        consoleScreenY = csbi.dwSize.Y;
        //consoleScreenX = SCREEN_WIDTH; // 콘솔 스크린 폭
        //consoleScreenY = SCREEN_HEIGHT; // 콘솔 스크린 높이

        // 실제 갱신할 화면 영역을 지정하자. 콘솔 크기 안쪽 사각형이라고 생각하면 됩니다.
        updateScreenSize.Left = consoleScreenSize.Left;
        updateScreenSize.Right = consoleScreenSize.Right;
        updateScreenSize.Bottom = consoleScreenSize.Bottom;
        updateScreenSize.Top = consoleScreenSize.Top;

        updateScreenX = consoleScreenX ;
        updateScreenY = consoleScreenY;
    }

    void ScreenFlipping()
    {
        SetConsoleActiveScreenBuffer(GetScreenHandle());
        bScreenIndex = !bScreenIndex;
    }
    void PrintScreen(int x, int y, const char* string)
    {
        DWORD dw;
        COORD CursorPosition = { x, y };
        SetConsoleCursorPosition(GetScreenHandle(), CursorPosition);
        WriteFile(GetScreenHandle(), string, strlen(string), &dw, NULL);
    }
    void PrintScreen(int x, int y, char* string)
    {
        DWORD dw;
        COORD CursorPosition = { x, y };
        SetConsoleCursorPosition(GetScreenHandle(), CursorPosition);
        WriteFile(GetScreenHandle(), string, strlen(string), &dw, NULL);
    }
    void ScreenClear()
    {
        COORD Coor = { updateScreenSize.Left, updateScreenSize.Top };
        DWORD dw;

        for (int y = 0; y < updateScreenY; y++)
        {
            Coor.Y = updateScreenSize.Top + y;

            FillConsoleOutputCharacter(GetScreenHandle(), ' ', updateScreenX, Coor, &dw);
        }
    }

    void ScreenRelease()
    {
        CloseHandle(hScreen[0]);
        CloseHandle(hScreen[1]);
    }

    // 콘솔이다 보니 픽셀 단위가아니라 라인 단위...한 글자씩 그려(?) 주렵니다.
    void ScreenDraw(int x, int y, const char c)
    {
        DWORD dw;
        COORD Cur = { x, y };
        char buffer[10];
        sprintf_s(buffer, "%c", c);

        SetConsoleCursorPosition(GetScreenHandle(), Cur);

        WriteFile(GetScreenHandle(), buffer, 1, &dw, NULL);
    }

    void ScreenDraw(int x, int y, const char* pStr)
    {
        DWORD dw;
        COORD Cur = { x, y };

        SetConsoleCursorPosition(GetScreenHandle(), Cur);

        WriteFile(GetScreenHandle(), pStr, strlen(pStr), &dw, NULL);
    }
    void ChoiceDraw(int x, int y, const char* pStr, bool highlight) {
        if(highlight)SetColor(12);
        PrintScreen(x,y,pStr);
        SetColor(1);
    }
    wchar_t* EncodeMap(wchar_t* pMap) // 세미콜론을 추가한 후 수정
    {
        for (int i = 0; i < MAP_PHEIGHT * MAP_PWIDTH; i++)
        {
            if (pMap[i] == '0')
            {
                pMap[i] = ' ';
            }
        }
        return pMap;
    }  // 세미콜론이 끝에 추가됨.

    void OpenTextAndWrite(int x, int y, const char* fileName) {
        FILE* pFile = nullptr;
        char* string = (char*)malloc(MAP_WIDTH + 1);  // +1은 널 종료 문자 추가를 위해;
        size_t length;
        int count = 0;
        fopen_s(&pFile, fileName, "rb");
        if (pFile == nullptr) {
            return;
        }
        // 파일 끝까지 반복
        fgets(string, MAP_WIDTH, pFile);
        
        length = strlen(string);
        // 동적으로 메모리 할당
        //string = (char*)malloc(length + 1);  // +1은 널 종료 문자 추가를 위해

        while (fgets(string, length+1, pFile) != NULL) {
            length = strlen(string);

            if (string[length - 1] == '\n')
                string[length - 1] = '\0';

            PrintScreen(x, y, string);
            y++;
        }
        free(string);
        fclose(pFile);
    }
    void OpenTextAndWriteAnim(int x, int y, const char* dirPath)
    {
        

    }
    void DrawGameText(int* menuFlag, COORD* curPlayerPos, int* curIndex, int* curEnemy, PLAYER* player, PLAYER* enemy)
    {
        switch (*menuFlag) {
        case TITLE:
            //EncodeMap(temp);
            RenderTitle(curIndex, curPlayerPos);
            PrintScreen(curPlayerPos->X - 3,curPlayerPos->Y, ">>");
            *menuFlag = TITLE;
            break;
        case HEROCHOICE:
            RenderHeroChoice(curIndex, curPlayerPos);
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            *menuFlag = HEROCHOICE;
            break;
        case MAIN:
            //EncodeMap(temp);
            RenderMain(curIndex, curPlayerPos, enemy);
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            for (int i = 40; i < SCREEN_HEIGHT - 1; i++)
            {
                if (i == 40 || i == SCREEN_HEIGHT - 2)
                {
                    for (int j = 0; j < SCREEN_WIDTH; j++)
                    {
                        PrintScreen(j, i, "-");
                    }
                }
                else
                {
                    PrintScreen(0, i, "|");
                    PrintScreen(SCREEN_WIDTH - 1, i, "|");
                }
            }
            *menuFlag = MAIN;
            break;
        case BATTLE:
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            for (int i = 40; i <= SCREEN_HEIGHT - 1; i++)
            {
                if (i == 40 || i == SCREEN_HEIGHT - 2)
                {
                    for (int j = 0; j < SCREEN_WIDTH; j++)
                    {
                        PrintScreen(j, i, "-");
                    }
                }
                else
                {
                    PrintScreen(0, i, "|");
                    PrintScreen(200, i, "|");
                }
            }
            *menuFlag = BATTLE;
            break; 
        case BATTLE_SKILL:
            //EncodeMap(temp);
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            *menuFlag = BATTLE_SKILL;
            break;
        case BATTLE_ITEM:
            //EncodeMap(temp);
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            *menuFlag = BATTLE_ITEM;
            break;
        case BATTLE_END:
            //EncodeMap(temp);
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            *menuFlag = BATTLE_END;
            break;
        case END:
            //EncodeMap(temp);
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            *menuFlag = END;
            break;
        default:
            DrawBorder();
            break;
        }
    }
    void SetColor(unsigned short color)
    {
        SetConsoleTextAttribute(GetScreenHandle(), color);
    }

    void DrawBorder()
    {
        // 위쪽 라인. Y 값이 고정 된다.
        for (int x = updateScreenSize.Left - 1; x < updateScreenSize.Right + 1; x++)
        {
            ScreenDraw(x, updateScreenSize.Top - 1, '#');
        }

        // 아래쪽 라인. Y 값이 고정 된다.
        for (int x = updateScreenSize.Left - 1; x < updateScreenSize.Right + 1; x++)
        {
            ScreenDraw(x, updateScreenSize.Bottom + 1, '#');
        }

        // 왼쪽 라인, X 값이 고정 된다.
        for (int y = updateScreenSize.Top - 1; y < updateScreenSize.Bottom + 1; y++)
        {
            ScreenDraw(updateScreenSize.Left - 1, y, '#');
        }

        // 오른쪽 라인, X 값이 고정 된다.
        for (int y = updateScreenSize.Top - 1; y < updateScreenSize.Bottom + 1; y++)
        {
            ScreenDraw(updateScreenSize.Right + 1, y, '#');
        }
    }
    void RenderTitle(int* choiceNum, COORD* curPlayerPos) {
        if (*choiceNum == 0) {
            ChoiceDraw(POS1, curPlayerPos->Y, "Game Start", true);
            ChoiceDraw(POS2, curPlayerPos->Y, "How To Play", false);
            ChoiceDraw(POS3, curPlayerPos->Y, "Game Info", false);
        }
        else if (*choiceNum == 1) {
            ChoiceDraw(POS1, curPlayerPos->Y, "Game Start", false);
            ChoiceDraw(POS2, curPlayerPos->Y, "How To Play", true);
            ChoiceDraw(POS3, curPlayerPos->Y, "Game Info", false);
        }
        else {
            ChoiceDraw(POS1, curPlayerPos->Y, "Game Start", false);
            ChoiceDraw(POS2, curPlayerPos->Y, "How To Play", false);
            ChoiceDraw(POS3, curPlayerPos->Y, "Game Info", true);
        }
    }
    void RenderHeroChoice(int* choiceNum, COORD* curPlayerPos)
    {
        if (*choiceNum == 0) {
            ChoiceDraw(POS1, curPlayerPos->Y, "Worrior", true);
            ChoiceDraw(POS2, curPlayerPos->Y, "Thief", false);
            ChoiceDraw(POS3, curPlayerPos->Y, "Wizard", false);
        }
        else if (*choiceNum == 1) {
            ChoiceDraw(POS1, curPlayerPos->Y, "Worrior", false);
            ChoiceDraw(POS2, curPlayerPos->Y, "Thief", true);
            ChoiceDraw(POS3, curPlayerPos->Y, "Wizard", false);
        }
        else {
            ChoiceDraw(POS1, curPlayerPos->Y, "Worrior", false);
            ChoiceDraw(POS2, curPlayerPos->Y, "Thief", false);
            ChoiceDraw(POS3, curPlayerPos->Y, "Wizard", true);
            OpenTextAndWrite(80, 5, ".\\Images\\Wizard1.txt");
        }
    }
    void RenderMain(int* choiceNum, COORD* curPlayerPos, PLAYER* enemy)
    {
        if (*choiceNum == 0) {
            ChoiceDraw(POS1, curPlayerPos->Y, enemy[0].job_name, true);
            ChoiceDraw(POS2, curPlayerPos->Y, enemy[1].job_name, false);
        }
        else if (*choiceNum == 1) {
            ChoiceDraw(POS1, curPlayerPos->Y, enemy[0].job_name, false);
            ChoiceDraw(POS2, curPlayerPos->Y, enemy[1].job_name, true);
        }
    }

    /*void RenderBattle(int* choiceNum, COORD* curPlayerPos, PLAYER* enemy)
    {
        if (*choiceNum == 0) {
            ChoiceDraw(POS1, curPlayerPos->Y, enemy[0].job_name, true);
            ChoiceDraw(POS2, curPlayerPos->Y, enemy[1].job_name, false);
        }
        else if (*choiceNum == 1) {
            ChoiceDraw(POS1, curPlayerPos->Y, enemy[0].job_name, false);
            ChoiceDraw(POS2, curPlayerPos->Y, enemy[1].job_name, true);
        }
    }*/
};



