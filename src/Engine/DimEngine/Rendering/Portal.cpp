#include "Portal.h"

#include "../Core/GameObject.h"

void Portal::Update(f32 deltaTime, f32 gameTime)
{

	// Replace main camera with focused camera for recursive rendering

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

__inline Camera* Portal::GetViewCamera()
{
	return viewCamera;
}

__inline void Portal::SetMainCamera(Camera* camera)
{
	mainCamera = camera;
}

__inline void Portal::SetViewCamera(Camera* camera)
{
	viewCamera = camera;
	SetViewClipPlane();
}

__inline void Portal::SetExit(Portal * portal)
{
	exit = portal;
}

void Portal::SetViewClipPlane()
{
	if (!exit || !viewCamera)
		return;

	//XMVECTOR normal = XMVector3Rotate({ 0, 0, 1 }, exit->GetGameObject()->GetRotation());
	XMVECTOR normal = exit->GetGameObject()->GetForwardVector();

	//XMFLOAT4 clipPlane;
	//XMStoreFloat4(&clipPlane, normal);

	//clipPlane.w = -XMVectorGetX(XMVector3Dot(normal, exit->GetGameObject()->GetPosition()));

	//viewCamera->SetClipPlane(clipPlane);
	viewCamera->SetClipPlane(normal, exit->GetGameObject()->GetPosition());
}
