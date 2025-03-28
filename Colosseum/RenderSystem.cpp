#include <stdio.h>

#include "renderSystem.h"
#include "Define.h"
#include"CharacterLocal.h"

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
    void ChoiceDraw(int x, int y, const char* pStr, bool highlight, int color) {
        if(highlight)SetColor(color);
        PrintScreen(x,y,pStr);
        SetColor(15);
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
        if (fileName == nullptr)
        {
            return;
        }
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
    void OpenTextAndWrite(int x, int y, const char* fileName, bool isEnemy) {
        FILE* pFile = nullptr;
        char* string = (char*)malloc(MAP_WIDTH + 1);  // +1은 널 종료 문자 추가를 위해;
        size_t length;
        int count = 0;
        if (fileName == nullptr)
        {
            return;
        }
        fopen_s(&pFile, fileName, "rb");
        if (pFile == nullptr) {
            return;
        }
        // 파일 끝까지 반복
        fgets(string, MAP_WIDTH, pFile);

        length = strlen(string);
        // 동적으로 메모리 할당
        //string = (char*)malloc(length + 1);  // +1은 널 종료 문자 추가를 위해

        while (fgets(string, length + 1, pFile) != NULL) {
            length = strlen(string);

   
           /*if (isEnemy)
            {
                int start = 0;
                int end = length - 1;
                char temp;
                while (start < end) {
                    temp = string[start];
                    string[start] = string[end];
                    string[end] = temp;
                    start++;
                    end--;
                }
            }*/

            if (string[length - 1] == '\n')
                string[length - 1] = '\0';

            PrintScreen(x, y, string);
            y++;
        }
        free(string);
        fclose(pFile);
    }
    void OpenTextAndWriteAnim(int x, int y, const char** fileName, int count, int color, bool isEnemy)
    {
        SetColor(color);
        static int animCount = 0;
        if (animCount >= count)
        {
            animCount = 0;
        }
        if(isEnemy)
            OpenTextAndWrite(x, y, fileName[animCount], isEnemy);
        else
            OpenTextAndWrite(x, y, fileName[animCount]);
        animCount++;
        if (animCount > count - 1)
            animCount = 0;
        SetColor(WHITE);
    }

    void DrawHP(PLAYER* player, PLAYER* enemy)
    {
        char numStr1[5];
        char numStr2[5];
        sprintf_s(numStr1, "%d", player->hp);
        sprintf_s(numStr2, "%d", enemy->hp);
        for (int i = 0; i <= (player->hp / 2); i++)
        {
            ChoiceDraw(PLAYER_HP_POSX + i, PLAYER_HP_POSY, "|", true, 12);
        }
        for (int i = 0; i <= (enemy->hp / 2); i++)
        {
            ChoiceDraw(ENEMY_HP_POSX - i, ENEMY_HP_POSY, "|", true, 12);
        }
        PrintScreen(PLAYER_HP_POSX, PLAYER_HP_POSY + 1, "나의 HP : ");
        PrintScreen(PLAYER_HP_POSX + 12, PLAYER_HP_POSY + 1, numStr1);
        PrintScreen(ENEMY_HP_POSX - 12, ENEMY_HP_POSY + 1, "적 HP : ");
        PrintScreen(ENEMY_HP_POSX - 2, ENEMY_HP_POSY + 1, numStr2);
    }

    void DrawStateText(COORD* curPlayerPos, PLAYER* player, PLAYER* enemy, bool* UseAttack, bool* UseSkill, bool* UseItem)
    {
        char numStr[5];
        if (*UseAttack)
        {
            if (player->isPlayer)
            {
                if (player->JOB == ARCHER)
                {
                    if (enemy->poisonStack >= player->maxPoisonStk)
                        sprintf_s(numStr, "%d", player->maxPoisonStk);
                    else
                        sprintf_s(numStr, "%d", enemy->poisonStack);
                }
                else
                    sprintf_s(numStr, "%d", player->atkDamage);
                PrintScreen(POS1, DEF_Y, "당신은 상대에게 "); 
                PrintScreen(POS1 + 16, DEF_Y, numStr);
                PrintScreen(POS1 + 20, DEF_Y, "데미지를 주었다.");
                *UseAttack = false;
            }
            else
            {
                if (player->JOB == ARCHER)
                {
                    if(enemy->poisonStack >= player->maxPoisonStk)
                        sprintf_s(numStr, "%d", player->maxPoisonStk);
                    else
                        sprintf_s(numStr, "%d", enemy->poisonStack);
                    PrintScreen(POS1, DEF_Y, player->job_name);
                    PrintScreen(POS1 + 10, DEF_Y, "이(가) 당신에게 ");
                    PrintScreen(POS1 + 26, DEF_Y, numStr);
                    PrintScreen(POS1 + 30, DEF_Y, "데미지를 주었다.");
                    *UseAttack = false;
                }
                else
                {
                    sprintf_s(numStr, "%d", player->atkDamage);
                    PrintScreen(POS1, DEF_Y, player->job_name);
                    PrintScreen(POS1 + 10, DEF_Y, "이(가) 당신에게 ");
                    PrintScreen(POS1 + 26, DEF_Y, numStr);
                    PrintScreen(POS1 + 30, DEF_Y, "데미지를 주었다.");
                    *UseAttack = false;
                }
            }
        }
        else if (*UseSkill)
        {
            if (player->isPlayer)
            {
                if (curPlayerPos->X == POS1)
                {
                    PrintScreen(POS1, DEF_Y, "당신은 ");
                    PrintScreen(POS1 + 7, DEF_Y, player->skill[0].skillName);
                    PrintScreen(POS1 + 15, DEF_Y, " 을 사용했다.");
                    if (player->skill[0].skillDamage > 0)
                    {
                        sprintf_s(numStr, "%d", player->skill[0].skillDamage);
                        PrintScreen(POS1 + 28, DEF_Y, "( ");
                        PrintScreen(POS1 + 30, DEF_Y, numStr);
                        PrintScreen(POS1 + 34, DEF_Y, "데미지)");
                    }
                    *UseSkill = false;
                }
                else if (curPlayerPos->X == POS2)
                {
                    PrintScreen(POS1, DEF_Y, "당신은 ");
                    PrintScreen(POS1 + 7, DEF_Y, player->skill[1].skillName);
                    PrintScreen(POS1 + 15, DEF_Y, " 을 사용했다.");
                    if (player->skill[0].skillDamage > 0)
                    {
                        sprintf_s(numStr, "%d", player->skill[1].skillDamage);
                        PrintScreen(POS1 + 28, DEF_Y, "( ");
                        PrintScreen(POS1 + 30, DEF_Y, numStr);
                        PrintScreen(POS1 + 34, DEF_Y, "데미지)");
                    }
                    *UseSkill = false;
                }
            }
            else
            {
                PrintScreen(POS1, DEF_Y, player->job_name);
                PrintScreen(POS1 + 10, DEF_Y, "이(가) 스킬을 사용했다.");
                *UseSkill = false;
            }
        }
        else if (*UseItem)
        {
            if (player->isPlayer)
            {
                if (curPlayerPos->X == POS1)
                {
                    PrintScreen(POS1, DEF_Y, "당신은 ");
                    PrintScreen(POS1 + 7, DEF_Y, player->item[0].itemName);
                    PrintScreen(POS1 + 15, DEF_Y, " 을 사용했다.");
                    *UseItem = false;
                }
                else if (curPlayerPos->X == POS2)
                {
                    PrintScreen(POS1, DEF_Y, "당신은 ");
                    PrintScreen(POS1 + 7, DEF_Y, player->item[1].itemName);
                    PrintScreen(POS1 + 15, DEF_Y, " 을 사용했다.");
                    *UseItem = false;
                }
            }
            else
            {
                PrintScreen(POS1, DEF_Y, player->job_name);
                PrintScreen(POS1 + 10, DEF_Y, "이(가) 아이템을 사용했습니다.");
                *UseItem = false;
            }
        }
    }
    void DrawGameText(int* menuFlag, COORD* curPlayerPos, int* curIndex, int* curEnemy, PLAYER* player, PLAYER* enemy)
    {
        switch (*menuFlag) {
        case TITLE:
            RenderTitle(curPlayerPos);
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            *menuFlag = TITLE;
            break;
        case HEROCHOICE:
            RenderHeroChoice(curPlayerPos);
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            *menuFlag = HEROCHOICE;
            break;
        case MAIN:
            RenderMain(curPlayerPos, enemy);
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            *menuFlag = MAIN;
            break;
        case BATTLE:
            RenderBattle(curPlayerPos);
            PrintScreen(POS2 + 5, curPlayerPos->Y, "(남은 스킬 사용 횟수 : ");
            if (player->skillCount == 0)
            {
                if (player->JOB == KNIGHT)
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
            RenderBattle_Skill(curPlayerPos, player);
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
            RenderBattle_Item(curPlayerPos, player);
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
            RenderBattle_End(curPlayerPos);
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            *menuFlag = BATTLE_END;
            break;
        case END_CLEAR:
            RenderEnd(curPlayerPos);
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            *menuFlag = END_CLEAR;
            break;
        case END_GAMEOVER:
            OpenTextAndWrite(80, 5, ".\\Images\\GameOver.txt");
            RenderEnd(curPlayerPos);
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            *menuFlag = END_CLEAR;
            break;
        case HOWTOPLAY:
            PrintScreen((SCREEN_WIDTH/2) - 20, 5, "이 게임은 턴제 전투 게임 '콜로세움' 입니다.");
            PrintScreen((SCREEN_WIDTH/2) - 20, 6, "당신은 전사, 도적, 마법사 이 세 가지 직업 중");
            PrintScreen((SCREEN_WIDTH / 2) - 20, 7, "한 가지를 선택하여 플레이 할 수 있습니다.");
            PrintScreen((SCREEN_WIDTH / 2) - 20, 8, "당신이 선택하지 않은 두 직업은 당신의 상대가 되어 당신과");
            PrintScreen((SCREEN_WIDTH / 2) - 20, 9, "전투를 하게 됩니다. ");
            PrintScreen((SCREEN_WIDTH / 2) - 20, 10, "두 명의 상대를 모두 물리치면 게임에서 승리하게 됩니다.");
            RenderEnd(curPlayerPos);
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            *menuFlag = HOWTOPLAY;
            break;
        case GAMEINFO:
            PrintScreen((SCREEN_WIDTH / 2) - 20, 5, "게임 개발");
            PrintScreen((SCREEN_WIDTH / 2) - 20, 6, "조현승");
            PrintScreen((SCREEN_WIDTH / 2) - 20, 7, "김장후");
            PrintScreen((SCREEN_WIDTH / 2) - 20, 8, "Colosseum");
            RenderEnd(curPlayerPos);
            PrintScreen(curPlayerPos->X - 3, curPlayerPos->Y, ">>");
            *menuFlag = GAMEINFO;
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

    void RenderPLAYERS(PLAYER* player, PLAYER* enemy)
    {
        if (player->JOB == KNIGHT)
        {
            OpenTextAndWriteAnim(0, 5, KnightIdle, 16, CYAN, false);
        }
        else if (player->JOB == ARCHER)
        {
            OpenTextAndWriteAnim(0, 5, ArcherIdle, 24, GREEN, false);
        }
        else if (player->JOB == BERSERKER)
        {
            OpenTextAndWriteAnim(0, 5, BerserkerIdle, 23, RED, false);
        }
        if (enemy->JOB == KNIGHT)
        {
            OpenTextAndWriteAnim(100, 5, KnightIdle, 16, CYAN, true);
        }
        else if (enemy->JOB == ARCHER)
        {
            OpenTextAndWriteAnim(100, 5, ArcherIdle, 24, GREEN, true);
        }
        else if (enemy->JOB == BERSERKER)
        {
            OpenTextAndWriteAnim(100, 5, BerserkerIdle, 23, RED, true);
        }
    }

    void RenderTitle(COORD* curPlayerPos) {
        OpenTextAndWrite(10, 10, TitleLogo);
        if (curPlayerPos->X == POS1) {
            ChoiceDraw(POS1, curPlayerPos->Y, "Game Start", true, 11);
            ChoiceDraw(POS2, curPlayerPos->Y, "How To Play", false, 11);
            ChoiceDraw(POS3, curPlayerPos->Y, "Game Info", false, 11);
        }
        else if (curPlayerPos->X == POS2) {
            ChoiceDraw(POS1, curPlayerPos->Y, "Game Start", false, 11);
            ChoiceDraw(POS2, curPlayerPos->Y, "How To Play", true, 11);
            ChoiceDraw(POS3, curPlayerPos->Y, "Game Info", false, 11);
        }
        else if (curPlayerPos->X == POS3) {
            ChoiceDraw(POS1, curPlayerPos->Y, "Game Start", false, 11);
            ChoiceDraw(POS2, curPlayerPos->Y, "How To Play", false, 11);
            ChoiceDraw(POS3, curPlayerPos->Y, "Game Info", true, 11);
        }
    }
    void RenderHeroChoice(COORD* curPlayerPos)
    {
        if (curPlayerPos->X == POS1) {
            ChoiceDraw(POS1, curPlayerPos->Y, "Knight", true, 11);
            ChoiceDraw(POS2, curPlayerPos->Y, "Archer", false, 11);
            ChoiceDraw(POS3, curPlayerPos->Y, "Berserker", false, 11);
            OpenTextAndWriteAnim((SCREEN_WIDTH / 2) - 50, 5, KnightIdle, 16, CYAN, false);
        }
        else if (curPlayerPos->X == POS2) {
            ChoiceDraw(POS1, curPlayerPos->Y, "Knight", false, 11);
            ChoiceDraw(POS2, curPlayerPos->Y, "Archer", true, 11);
            ChoiceDraw(POS3, curPlayerPos->Y, "Berserker", false, 11);
            OpenTextAndWriteAnim((SCREEN_WIDTH / 2) - 50, 5, ArcherIdle, 24, GREEN, false);
        }
        else if (curPlayerPos->X == POS3) {
            ChoiceDraw(POS1, curPlayerPos->Y, "Knight", false, 11);
            ChoiceDraw(POS2, curPlayerPos->Y, "Archer", false, 11);
            ChoiceDraw(POS3, curPlayerPos->Y, "Berserker", true, 11);
            OpenTextAndWriteAnim((SCREEN_WIDTH / 2) - 50, 5, BerserkerIdle, 23, RED, false);
        }
    }
    void RenderMain(COORD* curPlayerPos, PLAYER* enemy)
    {
        if (curPlayerPos->X == POS1) {
            ChoiceDraw(POS1, curPlayerPos->Y, enemy[0].job_name, true, 11);
            ChoiceDraw(POS2, curPlayerPos->Y, enemy[1].job_name, false, 11);
            if (enemy[0].JOB == KNIGHT)
            {
                OpenTextAndWriteAnim((SCREEN_WIDTH / 2) - 50, 5, KnightIdle, 16, CYAN, false);
            }
            else if (enemy[0].JOB == ARCHER)
            {
                OpenTextAndWriteAnim((SCREEN_WIDTH / 2) - 50, 5, ArcherIdle, 24, GREEN, false);
            }
            else if (enemy[0].JOB == BERSERKER)
            {
                OpenTextAndWriteAnim((SCREEN_WIDTH / 2) - 50, 5, BerserkerIdle, 23, RED, false);
            }
        }
        else if (curPlayerPos->X == POS2) {
            ChoiceDraw(POS1, curPlayerPos->Y, enemy[0].job_name, false, 11);
            ChoiceDraw(POS2, curPlayerPos->Y, enemy[1].job_name, true, 11);
            if (enemy[1].JOB == KNIGHT)
            {
                OpenTextAndWriteAnim((SCREEN_WIDTH / 2) - 50, 5, KnightIdle, 16, CYAN, false);
            }
            else if (enemy[1].JOB == ARCHER)
            {
                OpenTextAndWriteAnim((SCREEN_WIDTH / 2) - 50, 5, ArcherIdle, 24, GREEN, false);
            }
            else if (enemy[1].JOB == BERSERKER)
            {
                OpenTextAndWriteAnim((SCREEN_WIDTH / 2) - 50, 5, BerserkerIdle, 23, RED, false);
            }
        }
    }

    void RenderBattle(COORD* curPlayerPos)
    {
        if (curPlayerPos->X == POS1) {
            ChoiceDraw(POS1, curPlayerPos->Y, "공격", true, 11);
            ChoiceDraw(POS2, curPlayerPos->Y, "스킬", false, 11);
            ChoiceDraw(POS3, curPlayerPos->Y, "아이템", false, 11);
        }
        else if (curPlayerPos->X == POS2) {
            ChoiceDraw(POS1, curPlayerPos->Y, "공격", false, 11);
            ChoiceDraw(POS2, curPlayerPos->Y, "스킬", true, 11);
            ChoiceDraw(POS3, curPlayerPos->Y, "아이템", false, 11);
        }
        else if (curPlayerPos->X == POS3) {
            ChoiceDraw(POS1, curPlayerPos->Y, "공격", false, 11);
            ChoiceDraw(POS2, curPlayerPos->Y, "스킬", false, 11);
            ChoiceDraw(POS3, curPlayerPos->Y, "아이템", true, 11);
        }
    }

    void RenderBattle_Skill(COORD* curPlayerPos, PLAYER* player)
    {
        if (curPlayerPos->X == POS1)
        {
            ChoiceDraw(POS1, curPlayerPos->Y, player->skill[0].skillName, true, 11);
            ChoiceDraw(POS2, curPlayerPos->Y, player->skill[1].skillName, false, 11);
            ChoiceDraw(POS3, curPlayerPos->Y, "돌아가기", false, 11);
            PrintScreen(POS1, DEF_Y + 3, player->skill[0].description);
        }
        else if (curPlayerPos->X == POS2)
        {
            ChoiceDraw(POS1, curPlayerPos->Y, player->skill[0].skillName, false , 11);
            ChoiceDraw(POS2, curPlayerPos->Y, player->skill[1].skillName, true, 11);
            ChoiceDraw(POS3, curPlayerPos->Y, "돌아가기", false, 11);
            PrintScreen(POS1, DEF_Y + 3, player->skill[1].description);
        }
        else if (curPlayerPos->X == POS3)
        {
            ChoiceDraw(POS1, curPlayerPos->Y, player->skill[0].skillName, false, 11);
            ChoiceDraw(POS2, curPlayerPos->Y, player->skill[1].skillName, false, 11);
            ChoiceDraw(POS3, curPlayerPos->Y, "돌아가기", true, 11);
        }
    }
    void RenderBattle_Item(COORD* curPlayerPos, PLAYER* player)
    {
        if (curPlayerPos->X == POS1)
        {
            ChoiceDraw(POS1, curPlayerPos->Y, player->item[0].itemName, true, 11);
            ChoiceDraw(POS2, curPlayerPos->Y, player->item[1].itemName, false, 11);
            ChoiceDraw(POS3, curPlayerPos->Y, "돌아가기", false, 11);
            PrintScreen(POS1, DEF_Y + 3, player->item[0].description);
        }
        else if (curPlayerPos->X == POS2)
        {
            ChoiceDraw(POS1, curPlayerPos->Y, player->item[0].itemName, false, 11);
            ChoiceDraw(POS2, curPlayerPos->Y, player->item[1].itemName, true, 11);
            ChoiceDraw(POS3, curPlayerPos->Y, "돌아가기", false, 11);
            PrintScreen(POS1, DEF_Y + 3, player->item[1].description);
        }
        else if (curPlayerPos->X == POS3)
        {
            ChoiceDraw(POS1, curPlayerPos->Y, player->item[0].itemName, false, 11);
            ChoiceDraw(POS2, curPlayerPos->Y, player->item[1].itemName, false, 11);
            ChoiceDraw(POS3, curPlayerPos->Y, "돌아가기", true, 11);
        }
    }
    void RenderBattle_End(COORD* curPlayerPos)
    {
        if (curPlayerPos->X == POS1) {
            ChoiceDraw(POS1, curPlayerPos->Y, "Hp 50 회복", true, 11);
            ChoiceDraw(POS2, curPlayerPos->Y, "공격력 5 증가(도적이면 스택 최대 데미지 5 증가)", false, 11);
            ChoiceDraw(POS4, curPlayerPos->Y, "스턴 아이템 한개 획득", false, 11);
        }
        else if (curPlayerPos->X == POS2) {
            ChoiceDraw(POS1, curPlayerPos->Y, "Hp 50 회복", false, 11);
            ChoiceDraw(POS2, curPlayerPos->Y, "공격력 5 증가(도적이면 스택 최대 데미지 5 증가)", true, 11);
            ChoiceDraw(POS4, curPlayerPos->Y, "스턴 아이템 한개 획득", false, 11);
        }
        else if (curPlayerPos->X == POS3) {
            ChoiceDraw(POS1, curPlayerPos->Y, "Hp 50 회복", false, 11);
            ChoiceDraw(POS2, curPlayerPos->Y, "공격력 5 증가(도적이면 스택 최대 데미지 5 증가)", false, 11);
            ChoiceDraw(POS4, curPlayerPos->Y, "스턴 아이템 한개 획득", true, 11);
        }
    }
    void RenderEnd(COORD* curPlayerPos)
    {
        ChoiceDraw(POS1, curPlayerPos->Y, "타이틀 화면으로", true, 11);
    }
};



