#include "Portal.h"

#include "Core/GameObject.h"

void Portal::Update(f32 deltaTime, f32 gameTime)
{
	/*XMVECTOR posOffset = 
		XMVector3Rotate(
			XMVector3Normalize(
				XMVectorSubtract(
					mainCamera->GetGameObject()->GetPosition(), 
					gameObject->GetPosition())), 
			XMQuaternionInverse(gameObject->GetRotation()));*/

	XMVECTOR offset = XMVectorSubtract(mainCamera->GetGameObject()->GetPosition(), gameObject->GetPosition());
	XMVECTOR portalCamPos = XMVectorAdd(exit->GetGameObject()->GetPosition(), offset);

	viewCamera->GetGameObject()->SetPosition(portalCamPos);
	viewCamera->GetGameObject()->SetRotation(mainCamera->GetGameObject()->GetRotation());

	//viewCamera->GetGameObject()->SetForwardVector(XMVector3Rotate(posOffset, exit->GetGameObject()->GetRotation()));
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