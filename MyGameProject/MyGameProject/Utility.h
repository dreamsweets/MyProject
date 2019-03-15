#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <Windows.h>

struct Vector3 {
	double x;
	double y;
	double z;
};

//커서 컨트롤을 위해 Borland 클래스 정의
class Borland {
public:
	static int wherex() {
		CONSOLE_SCREEN_BUFFER_INFO csbilnfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbilnfo);
		return csbilnfo.dwCursorPosition.X;
	}
	static int wherey()
	{
		CONSOLE_SCREEN_BUFFER_INFO  csbiInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
		return csbiInfo.dwCursorPosition.Y;
	}
	static void gotoxy(int x, int y)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), _COORD{ (SHORT)x, (SHORT)y });
	}
	static void gotoxy(const Vector3& pos)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), _COORD{ (SHORT)pos.x, (SHORT)pos.y });
	}
};
#endif // !_UTILITY_H_
