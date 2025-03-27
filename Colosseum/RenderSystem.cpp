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
        updateScreenSize.Left = consoleScreenSize.Left + 2;
        updateScreenSize.Right = consoleScreenSize.Right - 2;
        updateScreenSize.Bottom = consoleScreenSize.Bottom - 2;
        updateScreenSize.Top = consoleScreenSize.Top + 2;

        updateScreenX = consoleScreenX - 4;
        updateScreenY = consoleScreenY - 4;
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
        char* string = (char*)malloc(200 + 1);  // +1은 널 종료 문자 추가를 위해;
        size_t length;
        int count = 0;
        fopen_s(&pFile, fileName, "rb");
        if (pFile == nullptr) {
            return;
        }
        // 파일 끝까지 반복
        fgets(string, 200, pFile);
        length = strlen(string);
        // 동적으로 메모리 할당
        string = (char*)malloc(length + 1);  // +1은 널 종료 문자 추가를 위해

        while (fgets(string, sizeof(string), pFile) != NULL) {
            if (string[length - 1] == '\n')
                string[length - 1] = '\0';

            PrintScreen(x, y++, string);
        }
        free(string);
        fclose(pFile);
    }
    void DrawGames(int* menuFlag, COORD* curPlayerPos, int* curIndex, int* curEnemy, PLAYER* player, PLAYER* enemy)
    {
        wchar_t* temp = OpenText("Maps\\Title.txt", MAP_PHEIGHT, MAP_PWIDTH);
        switch (*menuFlag) {
        case TITLE:
            OpenTextAndWrite(1, 1, "C:\\Users\\User\\Desktop\\KJH\\VisualStudio\\Project\\ConsolProject\\Colosseum\\Colosseum\\Images\\Wizard1.txt");
            //EncodeMap(temp);
            RenderTitle(curIndex, curPlayerPos);
            PrintScreen(curPlayerPos->X - 3,curPlayerPos->Y, ">>");
            *menuFlag = TITLE;
            break;
        case HEROCHOICE:
            temp = OpenText("Maps\\Title.txt", MAP_PHEIGHT, MAP_PWIDTH);
            //EncodeMap(temp);
            RenderHeroChoice(curIndex, curPlayerPos);
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            *menuFlag = HEROCHOICE;
            break;
        case MAIN:
            temp = OpenText("Maps\\Title.txt", MAP_PHEIGHT, MAP_PWIDTH);
            //EncodeMap(temp);
            RenderMain(curIndex, curPlayerPos, enemy);
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            *menuFlag = MAIN;
            break;
        case BATTLE:
            temp = OpenText("Maps\\Title.txt", MAP_PHEIGHT, MAP_PWIDTH);
            //EncodeMap(temp);
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            *menuFlag = BATTLE;
            break; 
        case BATTLE_SKILL:
            temp = OpenText("Maps\\Title.txt", MAP_PHEIGHT, MAP_PWIDTH);
            //EncodeMap(temp);
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            *menuFlag = BATTLE_SKILL;
            break;
        case BATTLE_ITEM:
            temp = OpenText("Maps\\Title.txt", MAP_PHEIGHT, MAP_PWIDTH);
            //EncodeMap(temp);
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            *menuFlag = BATTLE_ITEM;
            break;
        case BATTLE_END:
            temp = OpenText("Maps\\Title.txt", MAP_PHEIGHT, MAP_PWIDTH);
            //EncodeMap(temp);
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            *menuFlag = BATTLE_END;
            break;
        case END:
            temp = OpenText("Maps\\Title.txt", MAP_PHEIGHT, MAP_PWIDTH);
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
    //char* OpenText(char* fileName, int fileHeight, int fileWidth)
    //{
    //    FILE* fp = NULL;
    //    size_t read_size;
    //    if (0 == fopen_s(&fp, fileName, "r")) // FILE형 자료형으로 받아서 입력값을 찾아 ReadOnly로 받아온다.
    //        ;
    //    else
    //        return NULL;
    //    char* temp = (char*)malloc(sizeof(char) * fileHeight * fileWidth);

    //    if (temp == NULL)
    //        return 0;
    //    while ((read_size = fread(temp, sizeof(char), fileHeight * fileWidth, fp)) > 0)
    //    {
    //    }
    //    *(temp + fileHeight * fileWidth - 1) = '\0';

    //    fclose(fp);

    //    return temp;
    //}
    //char* OpenText(const char* fileName, int fileHeight, int fileWidth)
    //{
    //    FILE* fp = NULL;
    //    size_t read_size;
    //    if (0 == fopen_s(&fp, fileName, "r")) // FILE형 자료형으로 받아서 입력값을 찾아 ReadOnly로 받아온다.
    //        ;
    //    else
    //        return NULL;
    //    char* temp = (char*)malloc(sizeof(char) * fileHeight * fileWidth);

    //    if (temp == NULL)
    //        return 0;
    //    while ((read_size = fread(temp, sizeof(char), fileHeight * fileWidth, fp)) > 0)
    //    {
    //    }
    //    *(temp + fileHeight * fileWidth - 1) = '\0';

    //    fclose(fp);

    //    return temp;
    //}
    //char* OpenText(const char* fileName, int fileHeight, int fileWidth)
    //{
    //    FILE* fp = NULL;
    //    size_t read_size;
    //    if (0 == fopen_s(&fp, fileName, "r")) // FILE형 자료형으로 받아서 입력값을 찾아 ReadOnly로 받아온다.
    //        ;
    //    else
    //        return NULL;
    //    char* temp = (char*)malloc(sizeof(char) * fileHeight * fileWidth);

    //    if (temp == NULL)
    //        return 0;
    //    while ((read_size = fread(temp, sizeof(char), fileHeight * fileWidth, fp)) > 0)
    //    {
    //    }
    //    *(temp + fileHeight * fileWidth - 1) = '\0';

    //    fclose(fp);

    //    return temp;
    //}
    wchar_t* OpenText(const char* fileName, int fileHeight, int fileWidth)
    {
        FILE* fp = NULL;
        size_t read_size;

        // 유니코드 파일을 읽기 위해 "rb" 모드로 파일을 엽니다.
        if (0 != fopen_s(&fp, fileName, "rb"))
        {
            return NULL; // 파일을 열 수 없으면 NULL 반환
        }
        // 파일 크기만큼 메모리 할당 (wchar_t는 2바이트씩 사용)
        wchar_t* temp = (wchar_t*)malloc(sizeof(wchar_t) * fileHeight * fileWidth);
        if (temp == NULL)
        {
            fclose(fp);
            return NULL;
        }
        // 파일에서 유니코드 데이터 읽기
        // fread를 사용하여 파일에서 데이터를 읽습니다.
        // 유니코드 데이터는 2바이트씩 읽어야 합니다.
        read_size = fread(temp, sizeof(wchar_t), fileHeight * fileWidth, fp);
        if (read_size == 0) // 읽은 내용이 없으면 NULL 반환
        {
            free(temp);
            fclose(fp);
            return NULL;
        }

        // 읽은 텍스트의 끝에 널 종료 문자 추가
        temp[read_size] = L'\0';

        fclose(fp);
        return temp;
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



