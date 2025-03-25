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
        CONSOLE_CURSOR_INFO cursorInfo = { 0, };
        cursorInfo.bVisible = 0; // 커서를 보일지 말지 결정(0이면 안보임, 0제외 숫자 값이면 보임)
        cursorInfo.dwSize = 1; // 커서의 크기를 결정 (1~100 사이만 가능)
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);


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
    void ChoiceDraw(int x, int y, const char* text, bool highlight) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        // 콘솔 화면 크기 설정
        const int width = 80;  // 콘솔 창 가로 크기 (적당히 설정)
        const int height = 25; // 콘솔 창 세로 크기 (적당히 설정)

        CHAR_INFO buffer[width * height]; // 화면 버퍼

        // 버퍼 초기화 (기본 공백 문자로 설정)
        for (int i = 0; i < width * height; ++i) {
            buffer[i].Char.UnicodeChar = L' '; // 공백 문자
            buffer[i].Attributes = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED; // 텍스트 색상 (흰색)
        }

        // 텍스트를 CHAR_INFO 배열에 배치
        int bufferIndex = y * width + x;
        for (int i = 0; text[i] != L'\0'; ++i) {
            if (text[i] == L'\n') {
                bufferIndex += width; // 줄바꿈 처리
            }
            else {
                buffer[bufferIndex].Char.UnicodeChar = text[i];
                bufferIndex++;
            }
        }
    }
    wchar_t* EncodeMap(wchar_t* pMap) // 세미콜론을 추가한 후 수정
    {
        for (int i = 0; i < MAP_HEIGHT * MAP_PWIDTH; i++)
        {
            if (pMap[i] == '0')
            {
                pMap[i] = ' ';
            }
        }
        return pMap;
    }  // 세미콜론이 끝에 추가됨.


    void DrawGames(int Stage, int* menuFlag)
    {
        wchar_t* temp = OpenText("Maps\\Title.txt", MAP_HEIGHT, MAP_PWIDTH);
        switch (Stage) {
        case TITLE:
            temp = OpenText("Maps\\Title.txt", MAP_HEIGHT, MAP_PWIDTH);
            EncodeMap(temp);
            RenderTitle(0);
            *menuFlag = TITLE;
            break;
        case HEROCHOICE:
            temp = OpenText("Maps\\Title.txt", MAP_HEIGHT, MAP_PWIDTH);
            EncodeMap(temp);
            *menuFlag = HEROCHOICE;
            break;
        case MAIN:
            temp = OpenText("Maps\\Title.txt", MAP_HEIGHT, MAP_PWIDTH);
            EncodeMap(temp);
            *menuFlag = MAIN;
            break;
        case BATTLE:
            temp = OpenText("Maps\\Title.txt", MAP_HEIGHT, MAP_PWIDTH);
            EncodeMap(temp);
            *menuFlag = BATTLE;
            break;
        case END:
            temp = OpenText("Maps\\Title.txt", MAP_HEIGHT, MAP_PWIDTH);
            EncodeMap(temp);
            *menuFlag = END;
        default:
            DrawBorder();
            break;
        }
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

    struct ChoiceMSG {
        int xPos;
        int yPos;
        char text[15];
    }choiceMSG[3];
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
    void RenderTitle(int* choiceNum) {
        if (*choiceNum == 0) {
            ChoiceDraw(choiceMSG[0].yPos, choiceMSG[0].xPos, choiceMSG[0].text, true);
            ChoiceDraw(choiceMSG[0].yPos, choiceMSG[0].xPos, choiceMSG[0].text, false);
            ChoiceDraw(choiceMSG[0].yPos, choiceMSG[0].xPos, choiceMSG[0].text, false);
        }
        else if (*choiceNum == 1) {
            ChoiceDraw(choiceMSG[1].yPos, choiceMSG[1].xPos, choiceMSG[1].text, false);
            ChoiceDraw(choiceMSG[1].yPos, choiceMSG[1].xPos, choiceMSG[1].text, true);
            ChoiceDraw(choiceMSG[1].yPos, choiceMSG[1].xPos, choiceMSG[1].text, false);
        }
        else {
            ChoiceDraw(choiceMSG[2].yPos, choiceMSG[2].xPos, choiceMSG[2].text, false);
            ChoiceDraw(choiceMSG[2].yPos, choiceMSG[2].xPos, choiceMSG[2].text, false);
            ChoiceDraw(choiceMSG[2].yPos, choiceMSG[2].xPos, choiceMSG[2].text, true);
        }
    }
};



