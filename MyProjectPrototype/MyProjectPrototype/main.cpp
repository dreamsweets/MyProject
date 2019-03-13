#include <iostream>
#include <vector>
#include <conio.h>

#include "Object.h"
#include "Renderer.h"
#include "InputManager.h"
using namespace std;

int main() {
	Renderer& renderer = Renderer::getInstance();
	//Initialization �ʱ�ȭ
	Object player("^6^", "player", { 0,0,0 }, { 0,0,0 }, { 100,100,100 });
	Object enemy(">0<", "enemy", { 20,0,0 }, { 0,0,0 }, { 100,100,100 });
	//���� ���� ���� 
	while (true) {
		renderer.Clear();
		if (InputManager::GetKeyDown(KeyCode::Escape)) break;//�Է�
		//������Ʈ�� �����ؾ��� �� �� ������Ʈ
		player.Update();
		enemy.Draw();
		renderer.Render();
		Sleep(1000);
		InputManager::endOfFrame();
	}

	//Decommissioning
	return 0;
}