#include "GameObject.h"

DimEngine::GameObject * DimEngine::GameComponent::GetGameObject()
{
	return gameObject;
}

bool DimEngine::GameComponent::IsActive()
{
	return isActive && gameObject->IsActive();
}

bool DimEngine::GameComponent::IsActiveSelf()
{
	return isActive;
}

void DimEngine::GameComponent::SendMessage_(const Message & message)
{
	gameObject->SendMessage_(message);
}

void DimEngine::GameComponent::SendMessageUp(const Message & message, u32 level)
{
	gameObject->SendMessageUp(message, level);
}

void DimEngine::GameComponent::SendMessageDown(const Message & message, u32 level)
{
	gameObject->SendMessageDown(message, level);
}
