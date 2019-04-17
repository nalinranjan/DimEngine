#include "Portal.h"

#include "Core/GameObject.h"

void Portal::Update(f32 deltaTime, f32 gameTime)
{
	XMVECTOR offset = XMVectorSubtract(mainCamera->GetGameObject()->GetPosition(), gameObject->GetPosition());
	XMVECTOR rotOffset = XMQuaternionInverse(
		XMQuaternionNormalizeEst(XMQuaternionMultiply(
			XMQuaternionInverse(gameObject->GetRotation()),
			exit->GetGameObject()->GetRotation())));

	XMVECTOR portalCamPos = XMVectorAdd(
		exit->GetGameObject()->GetPosition(),
		XMVector3Rotate(offset, rotOffset));


	viewCamera->GetGameObject()->SetPosition(portalCamPos);
	viewCamera->GetGameObject()->SetRotation(XMQuaternionMultiply(
		mainCamera->GetGameObject()->GetRotation(),
		rotOffset));
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