#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <string>

#include "Utility.h"
using namespace std;

class Renderer;
//기본이 되는 오브젝트 클래스. SRT정보와 이름을 가지고 있다. 나중엔 컴포넌트로 바꿔야 할 것 같지만 일단 모양 정보도 추가한다.
class Object {
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	std::string name;

	//나중에 수정해야함!
	std::string shape;
	Renderer& renderer;
public:
	Object(std::string shape, std::string name, Vector3 position, Vector3 rotation, Vector3 scale);

	virtual void Update();
	void Draw();
	void Move();
};

#endif // !_OBJECT_H_
