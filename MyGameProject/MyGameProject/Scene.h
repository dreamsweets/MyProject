#pragma once
#include <vector>
using namespace std;
class GameObject;
class Scene
{
	vector<GameObject*> gameObjects;
public:
	string name;
	Scene(const string& name = "Default Scene");
	~Scene();
	void loop();
	GameObject* Search(const string& name);
};

