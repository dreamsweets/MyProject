#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <string>

#include "Utility.h"
using namespace std;

class Renderer;
//�⺻�� �Ǵ� ������Ʈ Ŭ����. SRT������ �̸��� ������ �ִ�. ���߿� ������Ʈ�� �ٲ�� �� �� ������ �ϴ� ��� ������ �߰��Ѵ�.
class Object {
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	std::string name;

	//���߿� �����ؾ���!
	std::string shape;
	Renderer& renderer;
public:
	Object(std::string shape, std::string name, Vector3 position, Vector3 rotation, Vector3 scale);

	virtual void Update();
	void Draw();
	void Move();
};

#endif // !_OBJECT_H_
