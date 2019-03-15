/*
Input ó���� ���� �Ŵ��� Ŭ����. �ϴ� Ű���� �Է��� �޴� �� ��������.
*/

#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_

#include <map>
using namespace std;

enum class KeyCode {
	None = 0,
	Escape,
	LeftArrow,
	RightArrow,
	UpArrow,
	DownArrow,
	Space,
};

/*Ű���� �̺�Ʈ�� 3���� ����� KeyDown, KeyPress, KeyUp. 
���⼭ KeyPress�� ��� ������ ���� ���̱� ������ ������ �̺�Ʈ�� �߻�*/
class InputManager {
	static int major;
	static int minor;
	static map<KeyCode, pair<int, int>> ketCodeTable;
	static bool isOverlap;

public:
	InputManager() {}
	static void EndOfFrame();
	static bool GetKeyDown(KeyCode key);

};
#endif // !_INPUTMANAGER_H_
