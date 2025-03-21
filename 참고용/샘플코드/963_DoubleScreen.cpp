#include <windows.h>
#include <conio.h>

HANDLE g_hScreen[2]; //두 개의 화면버퍼를 담을 배열
int screen_cur = 0;

int Getkey(void);								//키입력
void PrintPos(int x, int y, const char* s);		//특정위치 출력

void CursorView(HANDLE hConsole, char show);	//커서숨기기

void ScreenInit();
void ScreenStart();								//스크린 
void ScreenEnd();								//스크린 
void ScreenClear(HANDLE hConsole);				//스크린 클리어

int main(void)
{
	CursorView(GetStdHandle(STD_OUTPUT_HANDLE), false);

	ScreenInit();

	int x = 5, y = 5;

	ScreenStart();
	PrintPos(x, y, "★");							//버퍼에 그리기
	ScreenEnd();

	while (1)
	{
		//input -----------------------------------
		int key = Getkey();
		if (key == 75) { if (x > 0) x = x - 1; }	// 좌 방향키
		if (key == 77) { if (x < 20) x = x + 1; }	// 우 방향키
		if (key == 72) { if (y > 0) y = y - 1; }	// 상 방향키
		if (key == 80) { if (y < 20) y = y + 1; }	// 하 방향키

		//-----------------------------------------

		//display ---------------------------------
		ScreenStart();

		PrintPos(x, y, "★");					 //버퍼에 그리기

		ScreenEnd();
	}
	return 0;
}

int Getkey(void)
{
	int ch = _getch();
	if (ch == 0 || ch == 224)	// 방향키의 경우 0 또는 224의 값이 먼저 입력됨
		ch = _getch();	// 그 다음에 해당 방향키에 따라 72(Up),80(Down),75(Left),77(Right) 값이 입력됨
	return ch;
}

void PrintPos(int x, int y, const char* s)
{
	HANDLE handle = g_hScreen[screen_cur];

	COORD Pos = { x * 2, y };
	//SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
	SetConsoleCursorPosition(handle, Pos);

	DWORD dw;
	WriteFile(handle, s, strlen(s), &dw, NULL);
}

void ScreenClear(HANDLE hConsole)
{
	COORD coordScreen = { 0, 0 };
	BOOL bSuccess;
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;
	bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	/* fill the entire screen with blanks */
	bSuccess = FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten);
	/* get the current text attribute */
	bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);
	/* now set the buffer's attributes accordingly */
	bSuccess = FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
	/* put the cursor at (0, 0) */
	bSuccess = SetConsoleCursorPosition(hConsole, coordScreen);
	return;
}
static HANDLE myConsole;

void ScreenInit() {
	g_hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	g_hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	CursorView(g_hScreen[0], false);
	CursorView(g_hScreen[1], false);
}

void ScreenStart() {
	screen_cur = ++screen_cur % 2;
	myConsole = g_hScreen[screen_cur];		  //버퍼
	ScreenClear(myConsole);					  //버퍼 Clear
}

void ScreenEnd() {
	SetConsoleActiveScreenBuffer(myConsole);  //버퍼를 활성화 함으로써 화면에 버퍼의 내용을 한번에 출력
}

void CursorView(HANDLE hConsole, char show) //커서숨기기
{
	//hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO ConsoleCursor;
	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;
	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}