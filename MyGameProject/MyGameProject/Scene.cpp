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

	//������Ʈ�� �߰��ؾ� �ϴµ�, �� ���� �������̽��� �����Ƿ� ���⼭ ���� �ڵ�� �ٿ���� �ҵ�
	//���ӿ�����Ʈ�� name�� �߰������� name���� ���ϴ� ������Ʈ�� �˻��ؼ� �ű⿡ �߰��ϴ� ������� �غ���.
	//�غ��ϱ� �ش� ������Ʈ�� ��� ����ؾ��ϴµ� �� ������ �˻��ؼ� ���� �� �� �����ε�.
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

