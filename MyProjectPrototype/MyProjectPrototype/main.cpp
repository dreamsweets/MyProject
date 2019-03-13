#include <iostream>
#include <vector>
#include <conio.h>

#include "Object.h"
#include "Renderer.h"
#include "InputManager.h"
using namespace std;

int main() {
	Renderer& renderer = Renderer::getInstance();
	//Initialization 초기화
	Object player("^6^", "player", { 0,0,0 }, { 0,0,0 }, { 100,100,100 });
	Object enemy(">0<", "enemy", { 20,0,0 }, { 0,0,0 }, { 100,100,100 });
	//게임 루프 구성 
	while (true) {
		renderer.Clear();
		if (InputManager::GetKeyDown(KeyCode::Escape)) break;//입력
		//오브젝트들 업뎃해야할 것 다 업데이트
		player.Update();
		enemy.Draw();
		renderer.Render();
		Sleep(1000);
		InputManager::endOfFrame();
	}

	//Decommissioning
	return 0;
}