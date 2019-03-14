#include <conio.h>
#include <iostream>

#include "InputManager.h"
#include "Utility.h"

using namespace std;

int InputManager::major = -1; // 0���� �θ� Ű���� �Է� ���� �Ŷ� �ߺ��Ǵϱ� -1�� �ʱ�ȭ
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
/*�־��� KeyCode�� �����ϴ� pair�� �Լ����� �Է� ���� pair�� ��ġ�ϴ��� �Ǻ��ϴ� �Լ�
��, Ű �Է¹޴� ��ɵ� ���ԵǾ� �ִ�.*/
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