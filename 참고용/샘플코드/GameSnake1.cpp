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

int x, y;			//x,y 좌표값을 저장 총 100개 
int dir;			//이동방향 저장 
int key;			//입력받은 키 저장 
int speed = 200;

const char* sign_head = "Ｏ";
const char* sign_body = "ㅇ";

void Title();
void Reset();
void Move(int dir);		//이동 

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

	PrintTo(15, 5, "+--------------------------+");
	PrintTo(15, 6, "         S N A K E         ");
	PrintTo(15, 7, "+--------------------------+");

	while (1)
	{
		if (_kbhit()) {			//키입력받음 
			key = _getch();
			if (key == ESC)	exit(0);	// ESC키면 종료 
			else			break;		// 아니면 그냥 계속 진행 
		}

		PrintTo(15, 10, " < PRESS ANY KEY TO START > "); Sleep(400);
		PrintTo(15, 10, "                            "); Sleep(400);
	}

	// init 
	system("cls");	//화면을 지움 
	CursorView(0);
	x = 15;			//시작 위치
	y = 8;
}

void Move(int dir)
{
	system("cls");	//화면을 지움 

	if (dir == LEFT)	--x;		//방향에 따라 새로운 좌표값을 변경 
	if (dir == RIGHT)	++x;
	if (dir == UP)		--y;
	if (dir == DOWN)	++y;

	PrintTo(x, y, sign_head);		//새로운 머리좌표값에 머리를 그림 
}

void Reset()
{
	system("cls");	//화면을 지움 

	while (_kbhit()) _getch(); //버퍼에 있는 키값을 버림 

	dir = RIGHT;		// 방향 초기화  
	speed = 200;	// 속도 초기화 

	CursorView(0);
}