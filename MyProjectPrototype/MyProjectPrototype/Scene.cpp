#include <vector>
#include <conio.h>
#include "Scene.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Renderer.h"
using namespace std;


Scene::Scene(const string& name) : name(name)
{
	auto player = new GameObject("player", { 20,0,0 });
	auto enemy = new GameObject("enemy", { 40,0,0 });

	gameObjects.push_back(player);
	gameObjects.push_back(enemy);

	//컴포넌트를 추가해야 하는데, 이 역시 인터페이스가 없으므로 여기서 직접 코드로 붙여줘야 할듯
	//게임오브젝트에 name을 추가했으니 name으로 원하는 오브젝트를 검색해서 거기에 추가하는 방식으로 해보자.
	//해보니까 해당 오브젝트를 계속 사용해야하는데 할 때마다 검색해서 쓰는 건 좀 오바인듯.
	player->AddComponent(new MeshRenderer("^3^"));
	player->AddComponent(new MoveManager());
	enemy->AddComponent(new MeshRenderer(">0<"));
}


Scene::~Scene()
{
}

void Scene::loop()
{
	while (true) {
		Renderer::getInstance().Clear();
		if (InputManager::GetKeyDown(KeyCode::Escape)) break;

		for (auto obj : gameObjects) {
			obj->Update();
		}

		Renderer::getInstance().Render();
		Sleep(66);
		InputManager::EndOfFrame();
	}
}

GameObject * Scene::Search(const string & name)
{
	for (auto obj : gameObjects) {
		if (obj->name == name)
			return obj;
	}
	return nullptr;
}

