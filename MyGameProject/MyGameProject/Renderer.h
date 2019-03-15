#ifndef _RENDERER_H_
#define _RENDERER_H_


//#include <vector>
#include <iostream>
#include "Object.h"
using namespace std;
/*
기능 : 화면 출력을 관리. 그 외에는 뭔가 넣으면 안됨.
*/
class Renderer {
	static const int screenSize = 119;
	char screen[screenSize + 1 + 1];
	Vector3 origin;
	Renderer() : origin({ 0,0,0 }) {Clear(); }
	static Renderer* renderer;
public:
	static Renderer& getInstance();
	//게임 오브젝트들의 위치, 모양 정보를 받아서 screen 버퍼에 저장
	void Draw(const Vector3& pos, const string& shape) {
		memcpy(screen+(int)pos.x, shape.c_str(), strlen(shape.c_str()));
	}
	void Render() {
		cout << screen << endl;
	}
	/*스크린 초기화*/
	void Clear() {
		Borland::gotoxy(origin);
		memset(screen, ' ', screenSize);
		screen[screenSize] = '\n';
		screen[screenSize +1] = '\0';
	}
};
#endif // !_RENDERER_H_