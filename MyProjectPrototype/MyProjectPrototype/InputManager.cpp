#include <conio.h>
#include <iostream>

#include "InputManager.h"
#include "Utility.h"

using namespace std;

int InputManager::major = -1; // 0으로 두면 키보드 입력 받은 거랑 중복되니까 -1로 초기화
int InputManager::minor = 0;
bool InputManager::isOverlap = false;

map<KeyCode, pair<int, int>> InputManager::ketCodeTable = {

	{ KeyCode::None,{ 0, 0 }},
	{ KeyCode::Escape, {0, 27 }},
	{ KeyCode::UpArrow,{ 224, 72 }},
	{ KeyCode::DownArrow,{ 224, 80 }},
	{ KeyCode::LeftArrow,{ 224, 75 }},
	{ KeyCode::RightArrow,{ 224, 77 }},
	{ KeyCode::Space, { 0, 32 }}

};
void InputManager::EndOfFrame()
{
	isOverlap = false;
	major = -1;
	minor = 0;
}
/*주어진 KeyCode에 대응하는 pair와 함수에서 입력 받은 pair가 일치하는지 판별하는 함수
즉, 키 입력받는 기능도 포함되어 있다.*/
bool InputManager::GetKeyDown(KeyCode key) {
	if (isOverlap == false) {
		if (_kbhit()) {
			isOverlap = true;

			major = _getch();
			if (major == 0 | major == 224) {
				minor = _getch();
			}
		}
	}
	auto keyPair = ketCodeTable[key];
	return (keyPair.first == major && keyPair.second == minor);
}