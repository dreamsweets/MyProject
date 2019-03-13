#include "Object.h"
#include "Renderer.h"

Object::Object(
	std::string shape,
	std::string name ,
	Vector3 position ,
	Vector3 rotation ,
	Vector3 scale
) :
	renderer(Renderer::getInstance()),
	shape(shape),
	name(name),
	position(position),
	rotation(rotation),
	scale(scale)
{}

void Object::Draw() {
	renderer.Draw(position, shape);
}