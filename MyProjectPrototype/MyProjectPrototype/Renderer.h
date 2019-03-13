#ifndef _RENDERER_H_
#define _RENDERER_H_

#define SCREEN_SIZE 119

//#include <vector>
#include <iostream>
#include "Object.h"
using namespace std;

class Renderer {
	char screen[SCREEN_SIZE + 1 + 1];

	Renderer() {Clear();}
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

	void Clear() {
		memset(screen, ' ', SCREEN_SIZE);
		screen[SCREEN_SIZE] = '\n';
		screen[SCREEN_SIZE+1] = '\0';
	}
};
#endif // !_RENDERER_H_
