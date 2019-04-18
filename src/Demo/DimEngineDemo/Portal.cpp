#include "Portal.h"

#include "Core/GameObject.h"

void Portal::Update(f32 deltaTime, f32 gameTime)
{
	XMVECTOR offset = XMVectorSubtract(mainCamera->GetGameObject()->GetPosition(), gameObject->GetPosition());
	offset = XMVector4Transform(offset, XMMatrixRotationQuaternion(XMQuaternionInverse(gameObject->GetRotation())));
	offset = -offset;

	XMVECTOR rotation = XMQuaternionMultiply(mainCamera->GetGameObject()->GetRotation(), XMQuaternionInverse(gameObject->GetRotation()));
	rotation = XMQuaternionMultiply(rotation, XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 0), XM_PI));

	viewCamera->GetGameObject()->SetLocalPosition(offset);
	viewCamera->GetGameObject()->SetLocalRotation(rotation);
}

__inline Portal* Portal::GetExit()
{
	return exit;
}

__inline void Portal::SetMainCamera(Camera* camera)
{
	mainCamera = camera;
}

__inline void Portal::SetViewCamera(Camera* camera)
{
	viewCamera = camera;
}

__inline void Portal::SetExit(Portal * portal)
{
	exit = portal;
}