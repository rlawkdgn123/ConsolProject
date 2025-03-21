//스네이크 게임 예시  // 응용하기 - 형태 변경, 색변경, 사운드 추가, 속도 변경, 규칙 변경 //클래스로 변경하기

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

#define MAP_X 3
#define MAP_Y 2
#define MAP_XSIZE 30
#define MAP_YSIZE 20

int x[100], y[100];	//x,y 좌표값을 저장 총 100개 
int food_x, food_y;	//food의 좌표값을 저장 
int length;			//몸길이를 기억 
int speed;			//게임 속도 
int score;			//점수 저장  --reset함수에 의해 초기화됨
int best_score = 0; //최고 점수 저장 --reset함수에 의해 초기화 되지 않음 
int last_score = 0; //마지막 점수 저장  --reset함수에 의해 초기화 되지 않음
int dir;			//이동방향 저장 
int key;			//입력받은 키 저장 
//int status_on = 0; // 개발자용 status 표시활성화 변수.. 게임중에 S키를 누르면 활성 

const char* sign_head = "Ｏ";
const char* sign_body = "ㅇ";
const char* sign_food = "☆"; //☆♪ //ㅂㅈㄷㄱㅅㅁㄴㅇㄹㅋㅌㅊ


void Title();		//게임 시작화면 
void Reset();		//게임을 초기화 
void DrawMap();		//게임판 테두리를 그림 
void Move(int dir);	//뱀머리를 이동 
void Pause();		//일시정지 
void GameOver();	//게임 오버를 확인 
void Food();		//음식 생성 
//void status();	// 개발자용 status표시  


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
		case PAUSE:		// P키를 누르면 일시정지 
			Pause();
			break;
		case ESC:		//ESC키를 누르면 프로그램 종료 
			exit(0);
		}
		key = 0;

		Move(dir);

		//if (status_on == 1) status(); // status표시 
	}
}

//---------------------------------------------------------------------------------
void Title()
{
	while (_kbhit()) _getch(); //버퍼에 있는 키값을 버림 

	DrawMap();    //맵 테두리를 그림 
	for (int i = MAP_Y + 1;i < MAP_Y + MAP_YSIZE - 1;i++) {
		for (int j = MAP_X + 1;j < MAP_X + MAP_XSIZE - 1;j++)
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
		PrintTo(MAP_X + (MAP_XSIZE / 2) - 7, MAP_Y + 9, " < PRESS ANY KEY TO START > ");
		Sleep(400);
		PrintTo(MAP_X + (MAP_XSIZE / 2) - 7, MAP_Y + 9, "                            ");
		Sleep(400);
	}

	Reset(); // 게임을 초기화  
}

void Reset()
{
	int i;
	system("cls");	//화면을 지움 
	DrawMap();		//맵 테두리를 그림 
	while (_kbhit()) _getch(); //버퍼에 있는 키값을 버림 

	dir = LEFT;		// 방향 초기화  
	speed = 200;	// 속도 초기화 
	length = 5;		// 뱀 길이 초기화 
	score = 0;		// 점수 초기화 

	for (i = 0; i < length; i++) {
		x[i] = MAP_XSIZE / 2 + i; y[i] = MAP_YSIZE / 2;
		PrintTo(MAP_X + x[i], MAP_Y + y[i], sign_body);  //뱀 몸통값 입력 
	}
	PrintTo(MAP_X + x[0], MAP_Y + y[0], sign_head);	//뱀 머리 그림 

	Food(); // food 생성  
	CursorView(0);
}

void DrawMap()		//맵 테두리 그리는 함수 
{
	for (int i = 0;i < MAP_XSIZE;i++) {
		PrintTo(MAP_X + i, MAP_Y, "■");
	}
	for (int i = MAP_Y + 1;i < MAP_Y + MAP_YSIZE - 1;i++) {
		PrintTo(MAP_X, i, "■");
		PrintTo(MAP_X + MAP_XSIZE - 1, i, "■");
	}
	for (int i = 0;i < MAP_XSIZE;i++) {
		PrintTo(MAP_X + i, MAP_Y + MAP_YSIZE - 1, "■");
	}
}

void Move(int dir)
{
	int i;

	if (x[0] == food_x && y[0] == food_y) { //food와 충돌했을 경우 
		score += 10;						//점수 증가 
		Food();								//새로운 food 추가 
		length++;							//길이증가 
		x[length - 1] = x[length - 2];		//새로 만든 몸통에 값 입력 
		y[length - 1] = y[length - 2];
	}
	if (x[0] == 0 || x[0] == MAP_XSIZE - 1 ||
		y[0] == 0 || y[0] == MAP_YSIZE - 1) //벽과 충돌했을 경우 
	{
		GameOver();
		return;		//GameOver에서 게임을 다시 시작하게 되면 여기서부터 반복 				
	}
	for (i = 1; i < length; i++) //자기몸과 충돌했는지 검사 
	{
		if (x[0] == x[i] && y[0] == y[i]) {
			GameOver();
			return;
		}
	}

	PrintTo(MAP_X + x[length - 1], MAP_Y + y[length - 1], "  ");	//몸통 마지막을 지움 
	for (i = length - 1;i > 0;i--) {								//몸통좌표를 한칸씩 옮김 
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

void Food()
{
	int food_crush_on = 0;		//food가 뱀 몸통좌표에 생길 경우 on 

	PrintTo(MAP_X, MAP_Y + MAP_YSIZE, " ");		//점수표시 
	std::cout << "  Score : " << score << "  Last Score : " << last_score << "  Best Score : " << best_score;

	while (1) {
		food_crush_on = 0;
		srand((unsigned)time(NULL));				//난수표생성 
		food_x = (rand() % (MAP_XSIZE - 2)) + 1;    //난수를 좌표값에 넣음 
		food_y = (rand() % (MAP_YSIZE - 2)) + 1;

		for (int i = 0;i < length;i++) {			//food가 뱀 몸통과 겹치는지 확인  
			if (food_x == x[i] && food_y == y[i]) {
				food_crush_on = 1;					//겹치면 food_crush_on 를 on 
				break;
			}
		}

		if (food_crush_on == 1) continue; //겹쳤을 경우 while문을 다시 시작 

		PrintTo(MAP_X + food_x, MAP_Y + food_y, sign_food); //안겹쳤을 경우 좌표값에 food를 찍고 
		speed -= 3; //속도 증가 
		break;

	}
}

void GameOver()	//게임종료 함수 
{
	PrintTo(MAP_X + (MAP_XSIZE / 2) - 6, MAP_Y + 5, "+----------------------+");
	PrintTo(MAP_X + (MAP_XSIZE / 2) - 6, MAP_Y + 6, "       GAME OVER        ");
	PrintTo(MAP_X + (MAP_XSIZE / 2) - 6, MAP_Y + 7, "+----------------------+");
	PrintTo(MAP_X + (MAP_XSIZE / 2) - 6, MAP_Y + 8, " YOUR SCORE : ");
	std::cout << (last_score = score);

	PrintTo(MAP_X + (MAP_XSIZE / 2) - 7, MAP_Y + 12, " Press any keys to restart.. ");

	if (score > best_score) {
		best_score = score;
		PrintTo(MAP_X + (MAP_XSIZE / 2) - 4, MAP_Y + 10, " BEST SCORE ");
	}
	Sleep(500);
	while (_kbhit()) _getch();
	key = _getch();
	Title();
}

void Pause()	// p키를 눌렀을 경우 게임을 일시 정지 
{
	while (1) {
		if (key == PAUSE) {
			PrintTo(MAP_X + (MAP_XSIZE / 2) - 9, MAP_Y, "< PAUSE : PRESS ANY KEY TO RESUME > ");
			Sleep(400);
			PrintTo(MAP_X + (MAP_XSIZE / 2) - 9, MAP_Y, "                                    ");
			Sleep(400);
		}
		else {
			DrawMap();
			return;
		}
		if (_kbhit()) {
			do {
				key = _getch();
			} while (key == 224);
		}
	}
}
