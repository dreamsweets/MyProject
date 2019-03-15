#ifndef _RENDERER_H_
#define _RENDERER_H_


//#include <vector>
#include <iostream>
#include "Object.h"
using namespace std;
/*
��� : ȭ�� ����� ����. �� �ܿ��� ���� ������ �ȵ�.
*/
class Renderer {
	static const int screenSize = 119;
	char screen[screenSize + 1 + 1];
	Vector3 origin;
	Renderer() : origin({ 0,0,0 }) {Clear(); }
	static Renderer* renderer;
public:
	static Renderer& getInstance();
	//���� ������Ʈ���� ��ġ, ��� ������ �޾Ƽ� screen ���ۿ� ����
	void Draw(const Vector3& pos, const string& shape) {
		memcpy(screen+(int)pos.x, shape.c_str(), strlen(shape.c_str()));
	}
	void Render() {
		cout << screen << endl;
	}
	/*��ũ�� �ʱ�ȭ*/
	void Clear() {
		Borland::gotoxy(origin);
		memset(screen, ' ', screenSize);
		screen[screenSize] = '\n';
		screen[screenSize +1] = '\0';
	}
};
#endif // !_RENDERER_H_