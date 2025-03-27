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
        if(highlight)SetColor(11);
        PrintScreen(x,y,pStr);
        SetColor(15);
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

    void OpenTextAndWrite(int x, int y, const char* fileName) {
        FILE* pFile = nullptr;
        char* string = (char*)malloc(MAP_WIDTH + 1);  // +1�� �� ���� ���� �߰��� ����;
        size_t length;
        int count = 0;
        fopen_s(&pFile, fileName, "rb");
        if (pFile == nullptr) {
            return;
        }
        // ���� ������ �ݺ�
        fgets(string, MAP_WIDTH, pFile);
        
        length = strlen(string);
        // �������� �޸� �Ҵ�
        //string = (char*)malloc(length + 1);  // +1�� �� ���� ���� �߰��� ����

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
    void OpenTextAndWriteAnim(int x, int y, const char** fileName, int count)
    {
        for (int i = 0; i < count; i++) {
            OpenTextAndWrite(x, y, fileName[i]);
        }
    }
    void DrawStateText(COORD* curPlayerPos, PLAYER* player, PLAYER* enemy, bool* UseAttack, bool* UseSkill, bool* UseItem)
    {
        char numStr[5];
        if (UseAttack)
        {
            if (player->isPlayer)
            {
                sprintf_s(numStr, "%d", player->atkDamage);
                PrintScreen(POS1, DEF_Y, "����� ��뿡�� "); 
                PrintScreen(POS1 + 16, DEF_Y, numStr);
                PrintScreen(POS1 + 20, DEF_Y, "�������� �־���.");
                *UseAttack = false;
            }
            else
            {
                sprintf_s(numStr, "%d", player->atkDamage);
                PrintScreen(POS1, DEF_Y, player->job_name);
                PrintScreen(POS1 + 10, DEF_Y, "��(��) ��ſ��� ");
                PrintScreen(POS1 + 26, DEF_Y, numStr);
                PrintScreen(POS1 + 30, DEF_Y, "�������� �־���.");
                *UseAttack = false;
            }
        }
        else if (UseSkill)
        {
            if (player->isPlayer)
            {
                if (curPlayerPos->X == POS1)
                {
                    PrintScreen(POS1, DEF_Y, "����� ");
                    PrintScreen(POS1 + 7, DEF_Y, player->skill[0].skillName);
                    PrintScreen(POS1 + 15, DEF_Y, " �� ����ߴ�.");
                    if (player->skill[0].skillDamage > 0)
                    {
                        sprintf_s(numStr, "%d", player->skill[0].skillDamage);
                        PrintScreen(POS1 + 28, DEF_Y, "( ");
                        PrintScreen(POS1 + 30, DEF_Y, numStr);
                        PrintScreen(POS1 + 34, DEF_Y, "������)");
                    }
                    *UseSkill = false;
                }
                else if (curPlayerPos->X == POS2)
                {
                    PrintScreen(POS1, DEF_Y, "����� ");
                    PrintScreen(POS1 + 7, DEF_Y, player->skill[1].skillName);
                    PrintScreen(POS1 + 15, DEF_Y, " �� ����ߴ�.");
                    if (player->skill[0].skillDamage > 0)
                    {
                        sprintf_s(numStr, "%d", player->skill[1].skillDamage);
                        PrintScreen(POS1 + 28, DEF_Y, "( ");
                        PrintScreen(POS1 + 30, DEF_Y, numStr);
                        PrintScreen(POS1 + 34, DEF_Y, "������)");
                    }
                    *UseSkill = false;
                }
            }
            else
            {
                PrintScreen(POS1, DEF_Y, player->job_name);
                PrintScreen(POS1 + 10, DEF_Y, "��(��) ��ų�� ����ߴ�.");
                *UseSkill = false;
            }
        }
        else if (UseItem)
        {
            if (player->isPlayer)
            {
                if (curPlayerPos->X == POS1)
                {
                    PrintScreen(POS1, DEF_Y, "����� ");
                    PrintScreen(POS1 + 7, DEF_Y, player->item[0].itemName);
                    PrintScreen(POS1 + 15, DEF_Y, " �� ����ߴ�.");
                    *UseItem = false;
                }
                else if (curPlayerPos->X == POS2)
                {
                    PrintScreen(POS1, DEF_Y, "����� ");
                    PrintScreen(POS1 + 7, DEF_Y, player->item[1].itemName);
                    PrintScreen(POS1 + 15, DEF_Y, " �� ����ߴ�.");
                    *UseItem = false;
                }
            }
            else
            {
                PrintScreen(POS1, DEF_Y, player->job_name);
                PrintScreen(POS1 + 10, DEF_Y, "��(��) �������� ����߽��ϴ�.");
                *UseItem = false;
            }
        }
    }
    void DrawGameText(int* menuFlag, COORD* curPlayerPos, int* curIndex, int* curEnemy, PLAYER* player, PLAYER* enemy)
    {
        switch (*menuFlag) {
        case TITLE:
            RenderTitle(curIndex, curPlayerPos);
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            *menuFlag = TITLE;
            break;
        case HEROCHOICE:
            RenderHeroChoice(curIndex, curPlayerPos);
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            *menuFlag = HEROCHOICE;
            break;
        case MAIN:
            RenderMain(curIndex, curPlayerPos, enemy);
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            *menuFlag = MAIN;
            break;
        case BATTLE:
            RenderBattle(curIndex, curPlayerPos);
            PrintScreen(POS2 + 5, curPlayerPos->Y, "(���� ��ų ��� Ƚ�� : ");
            if (player->skillCount == 0)
            {
                if (player->JOB == WARRIOR)
                    PrintScreen(POS2 + 28, curPlayerPos->Y, "1)");
                else
                    PrintScreen(POS2 + 28, curPlayerPos->Y, "3)");
            }
            else if (player->skillCount == 1)
                PrintScreen(POS2 + 28, curPlayerPos->Y, "2)");
            else if (player->skillCount == 2)
                PrintScreen(POS2 + 28, curPlayerPos->Y, "1)");
            else if (player->skillCount >= 3)
                PrintScreen(POS2 + 28, curPlayerPos->Y, "0)");
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            for (int i = 40; i <= SCREEN_HEIGHT - 1; i++)
            {
                if (i == 40 || i == SCREEN_HEIGHT - 1)
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
            *menuFlag = BATTLE;
            break;
        case BATTLE_SKILL:
            RenderBattle_Skill(curIndex, curPlayerPos, player);
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            for (int i = 40; i <= SCREEN_HEIGHT - 1; i++)
            {
                if (i == 40 || i == SCREEN_HEIGHT - 1)
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
            *menuFlag = BATTLE_SKILL;
            break;
        case BATTLE_ITEM:
            RenderBattle_Item(curIndex, curPlayerPos, player);
            PrintScreen(POS1 + 6, curPlayerPos->Y, " : ");
            if (player->item[0].itemcount == 2)
                PrintScreen(POS1 + 9, curPlayerPos->Y, "2");
            else if (player->item[0].itemcount == 1)
                PrintScreen(POS1 + 9, curPlayerPos->Y, "1");
            else if (player->item[0].itemcount == 0)
                PrintScreen(POS1 + 9, curPlayerPos->Y, "0");
            PrintScreen(POS2 + 4, curPlayerPos->Y, " : ");
            if (player->item[1].itemcount == 2)
                PrintScreen(POS2 + 7, curPlayerPos->Y, "2");
            else if (player->item[1].itemcount == 1)
                PrintScreen(POS2 + 7, curPlayerPos->Y, "1");
            else if (player->item[1].itemcount == 0)
                PrintScreen(POS2 + 7, curPlayerPos->Y, "0");
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            for (int i = 40; i <= SCREEN_HEIGHT - 1; i++)
            {
                if (i == 40 || i == SCREEN_HEIGHT - 1)
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
            *menuFlag = BATTLE_ITEM;
            break;
        case BATTLE_END:
            RenderBattle_End(curIndex, curPlayerPos);
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            *menuFlag = BATTLE_END;
            break;
        case END_CLEAR:
            RenderEnd(curIndex, curPlayerPos);
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            *menuFlag = END_CLEAR;
            break;
        case END_GAMEOVER:
            OpenTextAndWrite(80, 5, ".\\Images\\GameOver.txt");
            RenderEnd(curIndex, curPlayerPos);
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            *menuFlag = END_CLEAR;
            break;
        default:
            break;
        }
    }
    void SetColor(unsigned short color)
    {
        SetConsoleTextAttribute(GetScreenHandle(), color);
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
        const char* a[13] = {
            "Wizard_ATK_1",
            "Wizard_ATK_2",
            "Wizard_ATK_3",
            "Wizard_ATK_4",
            "Wizard_ATK_5",
            "Wizard_ATK_6",
            "Wizard_ATK_7",
            "Wizard_ATK_8",
            "Wizard_ATK_9",
            "Wizard_ATK_10",
            "Wizard_ATK_11",
            "Wizard_ATK_12",
            "Wizard_ATK_13"
        };
        OpenTextAndWriteAnim(1, 2, a, 13);
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
            ChoiceDraw(POS1, curPlayerPos->Y, "Warrior", true);
            ChoiceDraw(POS2, curPlayerPos->Y, "Thief", false);
            ChoiceDraw(POS3, curPlayerPos->Y, "Wizard", false);
        }
        else if (*choiceNum == 1) {
            ChoiceDraw(POS1, curPlayerPos->Y, "Warrior", false);
            ChoiceDraw(POS2, curPlayerPos->Y, "Thief", true);
            ChoiceDraw(POS3, curPlayerPos->Y, "Wizard", false);
        }
        else {
            ChoiceDraw(POS1, curPlayerPos->Y, "Warrior", false);
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

    void RenderBattle(int* choiceNum, COORD* curPlayerPos)
    {
        if (*choiceNum == 0) {
            ChoiceDraw(POS1, curPlayerPos->Y, "����", true);
            ChoiceDraw(POS2, curPlayerPos->Y, "��ų", false);
            ChoiceDraw(POS3, curPlayerPos->Y, "������", false);
        }
        else if (*choiceNum == 1) {
            ChoiceDraw(POS1, curPlayerPos->Y, "����", false);
            ChoiceDraw(POS2, curPlayerPos->Y, "��ų", true);
            ChoiceDraw(POS3, curPlayerPos->Y, "������", false);
        }
        else if (*choiceNum == 2) {
            ChoiceDraw(POS1, curPlayerPos->Y, "����", false);
            ChoiceDraw(POS2, curPlayerPos->Y, "��ų", false);
            ChoiceDraw(POS3, curPlayerPos->Y, "������", true);
        }
    }

    void RenderBattle_Skill(int* choiceNum, COORD* curPlayerPos, PLAYER* player)
    {
        if (*choiceNum == 0)
        {
            ChoiceDraw(POS1, curPlayerPos->Y, player->skill[0].skillName, true);
            ChoiceDraw(POS2, curPlayerPos->Y, player->skill[1].skillName, false);
            ChoiceDraw(POS3, curPlayerPos->Y, "���ư���", false);
        }
        else if (*choiceNum == 1)
        {
            ChoiceDraw(POS1, curPlayerPos->Y, player->skill[0].skillName, false);
            ChoiceDraw(POS2, curPlayerPos->Y, player->skill[1].skillName, true);
            ChoiceDraw(POS3, curPlayerPos->Y, "���ư���", false);
        }
        else if (*choiceNum == 2)
        {
            ChoiceDraw(POS1, curPlayerPos->Y, player->skill[0].skillName, false);
            ChoiceDraw(POS2, curPlayerPos->Y, player->skill[1].skillName, false);
            ChoiceDraw(POS3, curPlayerPos->Y, "���ư���", true);
        }
    }
    void RenderBattle_Item(int* choiceNum, COORD* curPlayerPos, PLAYER* player)
    {
        if (*choiceNum == 0)
        {
            ChoiceDraw(POS1, curPlayerPos->Y, player->item[0].itemName, true);
            ChoiceDraw(POS2, curPlayerPos->Y, player->item[1].itemName, false);
            ChoiceDraw(POS3, curPlayerPos->Y, "���ư���", false);
        }
        else if (*choiceNum == 1)
        {
            ChoiceDraw(POS1, curPlayerPos->Y, player->item[0].itemName, false);
            ChoiceDraw(POS2, curPlayerPos->Y, player->item[1].itemName, true);
            ChoiceDraw(POS3, curPlayerPos->Y, "���ư���", false);
        }
        else if (*choiceNum == 2)
        {
            ChoiceDraw(POS1, curPlayerPos->Y, player->item[0].itemName, false);
            ChoiceDraw(POS2, curPlayerPos->Y, player->item[1].itemName, false);
            ChoiceDraw(POS3, curPlayerPos->Y, "���ư���", true);
        }
    }
    void RenderBattle_End(int* choiceNum, COORD* curPlayerPos)
    {
        if (*choiceNum == 0) {
            ChoiceDraw(POS1, curPlayerPos->Y, "Hp 50 ȸ��", true);
            ChoiceDraw(POS2, curPlayerPos->Y, "���ݷ� 5 ����(�����̸� ���� �ִ� ������ 5 ����)", false);
            ChoiceDraw(POS3, curPlayerPos->Y, "���� ������ �Ѱ� ȹ��", false);
        }
        else if (*choiceNum == 1) {
            ChoiceDraw(POS1, curPlayerPos->Y, "Hp 50 ȸ��", false);
            ChoiceDraw(POS2, curPlayerPos->Y, "���ݷ� 5 ����(�����̸� ���� �ִ� ������ 5 ����)", true);
            ChoiceDraw(POS3, curPlayerPos->Y, "���� ������ �Ѱ� ȹ��", false);
        }
        else if (*choiceNum == 2) {
            ChoiceDraw(POS1, curPlayerPos->Y, "Hp 50 ȸ��", false);
            ChoiceDraw(POS2, curPlayerPos->Y, "���ݷ� 5 ����(�����̸� ���� �ִ� ������ 5 ����)", false);
            ChoiceDraw(POS3, curPlayerPos->Y, "���� ������ �Ѱ� ȹ��", true);
        }
    }
    void RenderEnd(int* choiceNum, COORD* curPlayerPos)
    {
        ChoiceDraw(POS1, curPlayerPos->Y, "Ÿ��Ʋ ȭ������", true);
    }
};



