#include <iostream>
#include <vector>
#include <conio.h>

#include "Object.h"
#include "Renderer.h"
using namespace std;

int main() {
	Renderer& renderer = Renderer::getInstance();
	//Initialization �ʱ�ȭ
	Object player("^6^", "player", { 0,0,0 }, { 0,0,0 }, { 100,100,100 });
	Object enemy(">0<", "enemy", { 20,0,0 }, { 0,0,0 }, { 100,100,100 });
	//���� ���� ���� 
	while (1/*escape*/) {
		renderer.Clear();
		//�Է�
		//������Ʈ�� �����ؾ��� �� �� ������Ʈ
		player.Draw();
		enemy.Draw();
		renderer.Render();
		Sleep(66);
	}

	//Decommissioning
	return 0;
}