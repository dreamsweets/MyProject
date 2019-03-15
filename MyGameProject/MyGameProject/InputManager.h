/*
Input 처리를 위한 매니저 클래스. 일단 키보드 입력을 받는 걸 구현하자.
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

/*키보드 이벤트는 3가지 방식임 KeyDown, KeyPress, KeyUp. 
여기서 KeyPress는 계속 누르고 있을 때이기 때문에 여러번 이벤트가 발생*/
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
