//스네이크 게임 예시

#include<windows.h>
#include<conio.h>
#include<time.h>
#include<iostream>

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define PAUSE 112
#define ESC 27

#define MAP_X 0
#define MAP_Y 0
#define MAP_XSIZE 30
#define MAP_YSIZE 20

int x[100], y[100];			//x,y 좌표값을 저장 총 100개 
int dir;			//이동방향 저장 
int key;			//입력받은 키 저장 
int speed = 200;
int length = 5;

const char* sign_head = "●";
const char* sign_body = "Ｏ";

void Title();
void Reset();
void Move(int dir);		//이동 
void DrawMap();

void CursorView(char show) //커서숨기기
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;
	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}
void PrintTo(int x, int y, const char* s) //x값을 2x로 변경, 좌표값에 바로 문자열을 입력
{
	COORD pos = { (short)(2 * x), (short)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	std::cout << s;
}

//---------------------------------------------------------------------------------
int main()
{
	Title();

	while (1)
	{
		if (_kbhit()) do { key = _getch(); } while (key == 224); //키 입력받음
		Sleep(speed);

		switch (key) { //입력받은 키를 파악하고 실행  
		case LEFT:		if (dir != RIGHT) dir = key;
			break;
		case RIGHT:		if (dir != LEFT) dir = key;
			break;
		case UP:		if (dir != DOWN) dir = key;
			break;
		case DOWN:		if (dir != UP) dir = key;
			break;
		case ESC:		//ESC키를 누르면 프로그램 종료 
			exit(0);
		}
		key = 0;


		Move(dir);
	}
}

//---------------------------------------------------------------------------------
void Title()
{
	while (_kbhit()) _getch(); //버퍼에 있는 키값을 버림 

	DrawMap();    //맵 테두리를 그림 
	for (int i = MAP_Y + 1; i < MAP_Y + MAP_YSIZE - 1; i++) {
		for (int j = MAP_X + 1; j < MAP_X + MAP_XSIZE - 1; j++)
			PrintTo(j, i, "  ");  // 맵 테두리 안쪽을 빈칸으로 채움 
	}

	PrintTo(MAP_X + (MAP_XSIZE / 2) - 7, MAP_Y + 5, "+--------------------------+");
	PrintTo(MAP_X + (MAP_XSIZE / 2) - 7, MAP_Y + 6, "         S N A K E         ");
	PrintTo(MAP_X + (MAP_XSIZE / 2) - 7, MAP_Y + 7, "+--------------------------+");
	PrintTo(MAP_X + (MAP_XSIZE / 2) - 7, MAP_Y + 9, " < PRESS ANY KEY TO START > ");
	PrintTo(MAP_X + (MAP_XSIZE / 2) - 7, MAP_Y + 11, "   ◇ ←,→,↑,↓ : Move    ");
	PrintTo(MAP_X + (MAP_XSIZE / 2) - 7, MAP_Y + 12, "   ◇ P : Pause         ");
	PrintTo(MAP_X + (MAP_XSIZE / 2) - 7, MAP_Y + 13, "   ◇ ESC : Quit        ");

	while (1)
	{
		if (_kbhit()) {			//키입력받음 
			key = _getch();
			if (key == ESC)	exit(0);	// ESC키면 종료 
			else			break;		// 아니면 그냥 계속 진행 
		}

		PrintTo(MAP_X + (MAP_XSIZE / 2) - 7, MAP_Y + 9, " < PRESS ANY KEY TO START > "); Sleep(400);
		PrintTo(MAP_X + (MAP_XSIZE / 2) - 7, MAP_Y + 9, "                            "); Sleep(400);
	}

	Reset(); // init 
}

void Move(int dir)
{
	//system("cls");	//화면을 지움 

	int i;

	PrintTo(MAP_X + x[length - 1], MAP_Y + y[length - 1], "  ");	//몸통 마지막을 지움 
	for (i = length - 1; i > 0; i--) {								//몸통좌표를 한칸씩 옮김 
		x[i] = x[i - 1];
		y[i] = y[i - 1];
	}
	PrintTo(MAP_X + x[0], MAP_Y + y[0], sign_body);		//머리가 있던 곳을 몸통으로 고침 
	if (dir == LEFT) --x[0];							//방향에 따라 새로운 머리좌표(x[0],y[0])값을 변경 
	if (dir == RIGHT) ++x[0];
	if (dir == UP) --y[0];
	if (dir == DOWN) ++y[0];
	PrintTo(MAP_X + x[i], MAP_Y + y[i], sign_head);		//새로운 머리좌표값에 머리를 그림 
}

void Reset()
{
	system("cls");	//화면을 지움 

	DrawMap();

	while (_kbhit()) _getch(); //버퍼에 있는 키값을 버림 
	dir = LEFT;		// 방향 초기화  
	speed = 200;		// 속도 초기화 

	//x[0] = MAP_XSIZE / 2;			// 시작 위치
	//y[0] = MAP_YSIZE / 2;

	for (int i = 0; i < length; i++) {
		x[i] = MAP_XSIZE / 2 + i; y[i] = MAP_YSIZE / 2;
		PrintTo(x[i], y[i], sign_body);					//뱀 몸통값 입력 
	}
	PrintTo(x[0], y[0], sign_head);						//뱀 머리 그림 

	CursorView(0);
}

void DrawMap()		//맵 테두리 그리는 함수 
{
	for (int i = 0; i < MAP_XSIZE; i++) {
		PrintTo(MAP_X + i, MAP_Y, "■");
	}
	for (int i = MAP_Y + 1; i < MAP_Y + MAP_YSIZE - 1; i++) {
		PrintTo(MAP_X, i, "■");
		PrintTo(MAP_X + MAP_XSIZE - 1, i, "■");
	}
	for (int i = 0; i < MAP_XSIZE; i++) {
		PrintTo(MAP_X + i, MAP_Y + MAP_YSIZE - 1, "■");
	}
}