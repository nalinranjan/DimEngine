#include "GameObject.h"

bool DimEngine::GameComponent::IsActive()
{
	return isActive;
}

DimEngine::GameObject * DimEngine::GameComponent::GetGameObject()
{
	return gameObject;
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
