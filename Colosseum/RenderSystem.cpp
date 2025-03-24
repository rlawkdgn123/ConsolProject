#include <stdio.h>
#include "renderSystem.h"
#include "Define.h"
#include "InputSystem.h"
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

        // �ܼ� ȭ�� ���� ũ�� ����
        COORD bufferSize = { (short)width, (short)height };
        SetConsoleScreenBufferSize(hConsole, bufferSize);

        // �ܼ� â ũ�� ����
        SMALL_RECT windowSize = { 0, 0, (short)(width - 1), (short)(height - 1) };
        SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

    }
    void InitScreen()
    {
        setConsoleSize(SCREEN_WIDTH, SCREEN_HEIGHT);

        // ȭ�� ���� 2���� �����.
        hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

        // �����̴� Ŀ���� �� ���� ��Ű��.
        CONSOLE_CURSOR_INFO cursorInfo = { 0, };
        cursorInfo.bVisible = 0; // Ŀ���� ������ ���� ����(0�̸� �Ⱥ���, 0���� ���� ���̸� ����)
        cursorInfo.dwSize = 1; // Ŀ���� ũ�⸦ ���� (1~100 ���̸� ����)
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);


        // �ܼ�â ũ�⸦ ���� ����.
        // ������ ���� �ִµ� �н� ������ �׳�, �������� ������ �ۼ�.
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
        //consoleScreenX = SCREEN_WIDTH; // �ܼ� ��ũ�� ��
        //consoleScreenY = SCREEN_HEIGHT; // �ܼ� ��ũ�� ����

        // ���� ������ ȭ�� ������ ��������. �ܼ� ũ�� ���� �簢���̶�� �����ϸ� �˴ϴ�.
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

    // �ܼ��̴� ���� �ȼ� �������ƴ϶� ���� ����...�� ���ھ� �׷�(?) �ַƴϴ�.
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
    void ChoiceDraw(int x, int y, const char* pStr, bool highLight)
    {
        DWORD dw;
        COORD Cur = { x, y };

        if (highLight)
        {
            // ������ �ؽ�Ʈ�� ����
            //SetColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        }
        SetConsoleCursorPosition(GetScreenHandle(), Cur);

        WriteFile(GetScreenHandle(), pStr, strlen(pStr), &dw, NULL);
    }

    char* EncodeMap(char* pMap)
    {
        for (int i = 0; i < MAP_HEIGHT * MAP_PWIDTH; i++)
        {
            if (pMap[i] == '0')
            {
                pMap[i] = ' ';
            }
        }
        return pMap;
    }

    void DrawGames(int Stage, int* menuFlag)
    {
        char* temp = OpenText("Maps\\Title.txt", MAP_HEIGHT, MAP_PWIDTH);
        switch(Stage) {
        case TITLE:
           temp = OpenText("Maps\\Title.txt", MAP_HEIGHT, MAP_PWIDTH);
           EncodeMap(temp);
           RenderTitle();
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
            *menuFlag =MAIN;
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
            DrawBorder;
            break;
        }
    }
    char* OpenText(char* fileName, int fileHeight, int fileWidth)
    {
        FILE* fp = NULL;
        size_t read_size;
        if (0 == fopen_s(&fp, fileName, "r")) // FILE�� �ڷ������� �޾Ƽ� �Է°��� ã�� ReadOnly�� �޾ƿ´�.
            ;
        else
            return NULL;
        char* temp = (char*)malloc(sizeof(char) * fileHeight * fileWidth);

        if (temp == NULL)
            return 0;
        while ((read_size = fread(temp, sizeof(char), fileHeight * fileWidth, fp)) > 0)
        {
        }
        *(temp + fileHeight * fileWidth - 1) = '\0';

        fclose(fp);

        return temp;
    }
    char* OpenText(const char* fileName, int fileHeight, int fileWidth)
    {
        FILE* fp = NULL;
        size_t read_size;
        if (0 == fopen_s(&fp, fileName, "r")) // FILE�� �ڷ������� �޾Ƽ� �Է°��� ã�� ReadOnly�� �޾ƿ´�.
            ;
        else
            return NULL;
        char* temp = (char*)malloc(sizeof(char) * fileHeight * fileWidth);

        if (temp == NULL)
            return 0;
        while ((read_size = fread(temp, sizeof(char), fileHeight * fileWidth, fp)) > 0)
        {
        }
        *(temp + fileHeight * fileWidth - 1) = '\0';

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
        // ���� ����. Y ���� ���� �ȴ�.
        for (int x = updateScreenSize.Left - 1; x < updateScreenSize.Right + 1; x++)
        {
            ScreenDraw(x, updateScreenSize.Top - 1, '#');
        }

        // �Ʒ��� ����. Y ���� ���� �ȴ�.
        for (int x = updateScreenSize.Left - 1; x < updateScreenSize.Right + 1; x++)
        {
            ScreenDraw(x, updateScreenSize.Bottom + 1, '#');
        }

        // ���� ����, X ���� ���� �ȴ�.
        for (int y = updateScreenSize.Top - 1; y < updateScreenSize.Bottom + 1; y++)
        {
            ScreenDraw(updateScreenSize.Left - 1, y, '#');
        }

        // ������ ����, X ���� ���� �ȴ�.
        for (int y = updateScreenSize.Top - 1; y < updateScreenSize.Bottom + 1; y++)
        {
            ScreenDraw(updateScreenSize.Right + 1, y, '#');
        }
    }
    void RenderTitle() {
        choiceMSG[0] = { 20, 35,  "Game Start" };
        choiceMSG[1] = { 20, 55,  "How To Play" };
        choiceMSG[2] = { 20, 75,  " Game Info  " };
        static int choiceNum = 0;
        if (choiceNum == 0) {
            ChoiceDraw(choiceMSG[0].yPos, choiceMSG[0].xPos, choiceMSG[0].text , true);
            ChoiceDraw(choiceMSG[0].yPos, choiceMSG[0].xPos, choiceMSG[0].text, false);
            ChoiceDraw(choiceMSG[0].yPos, choiceMSG[0].xPos, choiceMSG[0].text, false);
        }
        else if(choiceNum == 1) {
            ChoiceDraw(choiceMSG[0].yPos, choiceMSG[0].xPos, choiceMSG[0].text, false);
            ChoiceDraw(choiceMSG[0].yPos, choiceMSG[0].xPos, choiceMSG[0].text, true);
            ChoiceDraw(choiceMSG[0].yPos, choiceMSG[0].xPos, choiceMSG[0].text, false);
        }
        else {
            ChoiceDraw(choiceMSG[0].yPos, choiceMSG[0].xPos, choiceMSG[0].text, false);
            ChoiceDraw(choiceMSG[0].yPos, choiceMSG[0].xPos, choiceMSG[0].text, false);
            ChoiceDraw(choiceMSG[0].yPos, choiceMSG[0].xPos, choiceMSG[0].text, true);
        }
        if (USER_CMD_LEFT && choiceNum > 0) {
            choiceNum--;
        }else if(USER_CMD_RIGHT && choiceNum < 2){
            choiceNum++;
        }
    }
};



