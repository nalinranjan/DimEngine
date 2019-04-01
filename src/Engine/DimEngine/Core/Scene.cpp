#include <stack>

#include "Scene.h"

DimEngine::Scene* DimEngine::Scene::currentScene = nullptr;

DimEngine::Scene* DimEngine::Scene::GetCurrentScene()
{
	if (!currentScene)
		currentScene = new Scene();

	return currentScene;
}

DimEngine::Scene::Scene()
{
}

DimEngine::Scene::~Scene()
{
}

size_t DimEngine::Scene::AddRootObject(GameObject* gameObject)
{
	roots.push_back(gameObject);

	return roots.size() - 1;
}

bool DimEngine::Scene::RemoveRootObject(GameObject* gameObject)
{
	if (gameObject->parent)
		return false;

	for (auto it = roots.begin(); it != roots.end(); it++)
		if (*it == gameObject)
		{
			roots.erase(it);
			break;
		}

	return true;
}

void DimEngine::Scene::Update(f32 deltaTime, f32 currentTime)
{
	Stack<GameObject*> s;

	for (auto it = roots.begin(); it != roots.end(); it++)
		s.Push(*it);

	while (s.GetSize())
	{
		GameObject* gameObject = s.Pop();

		gameObject->Update(deltaTime, currentTime);

		for (auto it = gameObject->children.begin(); it != gameObject->children.end(); it++)
			if (static_cast<GameObject*>(*it)->IsActiveSelf())
				s.Push(static_cast<GameObject*>(*it));
	}
}

void DimEngine::Scene::LateUpdate(f32 deltaTime, f32 currentTime)
{
	Stack<GameObject*> s;

	for (auto it = roots.begin(); it != roots.end(); it++)
		s.Push(*it);

	while (s.GetSize())
	{
		GameObject* gameObject = s.Pop();

		gameObject->LateUpdate(deltaTime, currentTime);

		for (auto it = gameObject->children.begin(); it != gameObject->children.end(); it++)
			if (static_cast<GameObject*>(*it)->IsActiveSelf())
				s.Push(static_cast<GameObject*>(*it));
	}
}
