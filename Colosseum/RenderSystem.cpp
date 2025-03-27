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
        CONSOLE_CURSOR_INFO cursorInfo = { 0 };
        cursorInfo.bVisible = 0; // Ŀ���� ������ ���� ����(0�̸� �Ⱥ���, 0���� ���� ���̸� ����)
        cursorInfo.dwSize = 1; // Ŀ���� ũ�⸦ ���� (1~100 ���̸� ����)
        SetConsoleCursorInfo(hScreen[0], &cursorInfo);
        SetConsoleCursorInfo(hScreen[1], &cursorInfo);


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
    void PrintScreen(int x, int y, const char* string)
    {
        DWORD dw;
        COORD CursorPosition = { x, y };
        SetConsoleCursorPosition(GetScreenHandle(), CursorPosition);
        WriteFile(GetScreenHandle(), string, strlen(string), &dw, NULL);
    }
    void PrintScreens(int x, int y, char** string)
    {
        DWORD dw;
        int i = 0;
        // �� ���� ���������� ���
        while (string[i] != NULL) {
            COORD CursorPosition = { x, y + i };  // �� �ٸ��� y ���� ����
            SetConsoleCursorPosition(GetScreenHandle(), CursorPosition);
            WriteFile(GetScreenHandle(), string[i], strlen(string[i]), &dw, NULL);
            i++;
        }
    }
    void OpenTextAndWright(int raw, int col, const char* fileName)
    {
        FILE* pFile = nullptr;
        char** lines = new char* [raw];
        int lineCount = 0;
        fopen_s(&pFile, fileName, "rb");

        // ������ �迭 �Ҵ� ��x��
        for (int i = 0; i < raw; i++) {
            lines[i] = new char[col + 1];
            // == lines* = new char[];
        }
        //lines[raw-1][col] = '\0';

        // �޾ƿ���
        //int lineCount = 0;
        //for (int i = 0; i < raw; i++)
        //    for (int j = 0; j < col; j++)
        //        fscanf_s(pFile, "%c", &lines[i][j]);

        while (lineCount < raw && fgets(lines[lineCount], col + 1, pFile)) {
            // fgets()�� �ڵ����� '\0'�� �߰��ϹǷ� �߰����� ó�� �ʿ� ����
            lineCount++;
        }

        //for (int i = 0; i < raw; i++) {
        //    printf("%s\n", lines[i]);
        //}
        // ���
        PrintScreens(2, 2, lines);
        //PrintScreen(raw, col, lines);

        // �Ҵ� ����
        for (int i = 0; i < raw; i++)
            delete[] lines[i];
        delete[] lines;
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
    void ChoiceDraw(int x, int y, const char* pStr, bool highlight) {
        if(highlight)SetColor(12);
        PrintScreen(x,y,pStr);
        SetColor(1);
    }
    wchar_t* EncodeMap(wchar_t* pMap) // �����ݷ��� �߰��� �� ����
    {
        for (int i = 0; i < MAP_PHEIGHT * MAP_PWIDTH; i++)
        {
            if (pMap[i] == '0')
            {
                pMap[i] = ' ';
            }
        }
        return pMap;
    }  // �����ݷ��� ���� �߰���.


    void DrawGames(int* menuFlag, COORD* curPlayerPos, int* curIndex, int* curEnemy, PLAYER* player, PLAYER* enemy)
    {
        wchar_t* temp = OpenText("Maps\\Title.txt", MAP_PHEIGHT, MAP_PWIDTH);
        switch (*menuFlag) {
        case TITLE:
            //temp = OpenText("Maps\\Title.txt", MAP_PHEIGHT, MAP_PWIDTH);
            //EncodeMap(temp);
            OpenTextAndWright(10, 50, "C:\\Users\\User\\Desktop\\KJH\\VisualStudio\\Project\\ConsolProject\\Colosseum\\Colosseum\\Images\\Wizard1.txt");
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
    //    if (0 == fopen_s(&fp, fileName, "r")) // FILE�� �ڷ������� �޾Ƽ� �Է°��� ã�� ReadOnly�� �޾ƿ´�.
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
    //    if (0 == fopen_s(&fp, fileName, "r")) // FILE�� �ڷ������� �޾Ƽ� �Է°��� ã�� ReadOnly�� �޾ƿ´�.
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
    //    if (0 == fopen_s(&fp, fileName, "r")) // FILE�� �ڷ������� �޾Ƽ� �Է°��� ã�� ReadOnly�� �޾ƿ´�.
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

        // �����ڵ� ������ �б� ���� "rb" ���� ������ ���ϴ�.
        if (0 != fopen_s(&fp, fileName, "rb"))
        {
            return NULL; // ������ �� �� ������ NULL ��ȯ
        }
        // ���� ũ�⸸ŭ �޸� �Ҵ� (wchar_t�� 2����Ʈ�� ���)
        wchar_t* temp = (wchar_t*)malloc(sizeof(wchar_t) * fileHeight * fileWidth);
        if (temp == NULL)
        {
            fclose(fp);
            return NULL;
        }
        // ���Ͽ��� �����ڵ� ������ �б�
        // fread�� ����Ͽ� ���Ͽ��� �����͸� �н��ϴ�.
        // �����ڵ� �����ʹ� 2����Ʈ�� �о�� �մϴ�.
        read_size = fread(temp, sizeof(wchar_t), fileHeight * fileWidth, fp);
        if (read_size == 0) // ���� ������ ������ NULL ��ȯ
        {
            free(temp);
            fclose(fp);
            return NULL;
        }

        // ���� �ؽ�Ʈ�� ���� �� ���� ���� �߰�
        temp[read_size] = L'\0';

        fclose(fp);
        return temp;
    }

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



