#include <iostream>
#include <vector>
#include <conio.h>

#include "Object.h"
#include "Renderer.h"
using namespace std;

int main() {
	Renderer& renderer = Renderer::getInstance();
	//Initialization 초기화
	Object player("^6^", "player", { 0,0,0 }, { 0,0,0 }, { 100,100,100 });
	Object enemy(">0<", "enemy", { 20,0,0 }, { 0,0,0 }, { 100,100,100 });
	//게임 루프 구성 
	while (1/*escape*/) {
		renderer.Clear();
		//입력
		//오브젝트들 업뎃해야할 것 다 업데이트
		player.Draw();
		enemy.Draw();
		renderer.Render();
		Sleep(66);
	}

	//Decommissioning
	return 0;
}