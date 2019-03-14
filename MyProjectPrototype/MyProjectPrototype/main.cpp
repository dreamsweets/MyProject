#include <iostream>
#include <vector>
#include <conio.h>

//#include "Object.h"
//#include "Renderer.h"
//#include "InputManager.h"
#include "Scene.h"
using namespace std;

int main() {
	Scene scene("Main");
	scene.loop();
	return 0;
}