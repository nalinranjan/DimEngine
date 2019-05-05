#include <typeinfo>

#include "GameObject.h"
#include "Scene.h"

using namespace DimEngine;

DimEngine::GameObject::GameObject() : Transform(), components(4)
{
	index = Scene::GetCurrentScene()->AddRootObject(this);

	flag = FLAG_ACTIVE;
}

DimEngine::GameObject::GameObject(float x, float y, float z) : Transform(x, y, z), components(4)
{
	index = Scene::GetCurrentScene()->AddRootObject(this);

	flag = FLAG_ACTIVE;
}

DimEngine::GameObject::GameObject(XMVECTOR position, XMVECTOR rotation, XMVECTOR scale) : Transform(position, rotation, scale), components(4)
{
	index = Scene::GetCurrentScene()->AddRootObject(this);

	flag = FLAG_ACTIVE;
}

DimEngine::GameObject::~GameObject()
{
	isDestroyed = true;

	if (!Scene::GetCurrentScene()->RemoveRootObject(this))
		parent->__RemoveChild(this);

	for (auto it = components.begin(); it != components.end(); it++)
		delete it->second;

	for (auto it = children.begin(); it != children.end(); it++)
		delete (*it);
}

__inline GameObject* DimEngine::GameObject::GetParent()
{
	return static_cast<GameObject*>(parent);
}

__inline GameObject* DimEngine::GameObject::GetChild(size_t index)
{
	return static_cast<GameObject*>(Transform::GetChild(index));
}

bool DimEngine::GameObject::ContainTag(string _tag)
{
	auto it = tags.find(_tag);
	if (it == tags.end()) return false;
	return true;
}

void DimEngine::GameObject::AddTag(string _tag)
{
	tags.insert(_tag);
}

__inline bool DimEngine::GameObject::IsActive()
{
	return parent ? isActive && static_cast<GameObject*>(parent)->IsActive() : isActive;
}

__inline bool DimEngine::GameObject::IsActiveSelf()
{
	return isActive;
}

__inline bool DimEngine::GameObject::IsStatic()
{
	return isStatic;
}

__inline void DimEngine::GameObject::SetParent(GameObject* other)
{
	Scene::GetCurrentScene()->RemoveRootObject(this);

	return Transform::SetParent(other);
}

void DimEngine::GameObject::SetActive(bool flag)
{
	isActive = flag;
}

void DimEngine::GameObject::SendMessage_(const Message& message)
{
	HandleMessage(message);

	for (auto it = components.begin(); it != components.end(); it++)
		it->second->HandleMessage(message);
}

void DimEngine::GameObject::SendMessageUp(const Message& message, u32 level)
{
	HandleMessage(message);

	for (auto it = components.begin(); it != components.end(); it++)
		it->second->HandleMessage(message);

	if (level-- > 0 && parent)
		parent->SendMessageUp(message, level);
}

void DimEngine::GameObject::SendMessageDown(const Message& message, u32 level)
{
	HandleMessage(message);

	for (auto it = components.begin(); it != components.end(); it++)
		it->second->HandleMessage(message);

	if (level-- > 0)
		for (auto it = children.begin(); it != children.end(); it++)
			(*it)->SendMessageDown(message, level);
}

__inline void DimEngine::GameObject::Update(f32 deltaTime, f32 currentTime)
{
	for (auto it = components.begin(); it != components.end(); it++)
	{
		GameComponent* component = (*it).second;
		if (component->isActive)
			component->Update(deltaTime, currentTime);
	}
}

__inline void DimEngine::GameObject::LateUpdate(f32 deltaTime, f32 currentTime)
{
	for (auto it = components.begin(); it != components.end(); it++)
	{
		GameComponent* component = (*it).second;
		if (component->isActive)
			component->LateUpdate(deltaTime, currentTime);
	}
}

__inline void DimEngine::GameObject::OnBeginOverlapping(GameObject* other)
{
	for (auto it = components.begin(); it != components.end(); it++)
	{
		GameComponent* component = it->second;
		if (component->IsActiveSelf())
			component->OnBeginOverlapping(other);
	}
}

__inline void DimEngine::GameObject::OnOverlapping(GameObject* other)
{
	for (auto it = components.begin(); it != components.end(); it++)
	{
		GameComponent* component = it->second;
		if (component->IsActiveSelf())
			component->OnOverlapping(other);
	}
}

__inline void DimEngine::GameObject::OnEndOverlapping(GameObject* other)
{
	for (auto it = components.begin(); it != components.end(); it++)
	{
		GameComponent* component = it->second;
		if (component->IsActiveSelf())
			component->OnEndOverlapping(other);
	}
}
