#include "Portal.h"

#include "Core/GameObject.h"

void Portal::Update(f32 deltaTime, f32 gameTime)
{
	XMVECTOR posOffset = XMVectorSubtract(mainCamera->GetGameObject()->GetPosition(), gameObject->GetPosition());

	XMVECTOR exitPosition = exit->GetGameObject()->GetPosition();
	XMVECTOR rotOffset = XMQuaternionNormalizeEst(XMQuaternionMultiply(XMQuaternionInverse(gameObject->GetRotation()), exitPosition));

	viewCamera->GetGameObject()->SetPosition(XMVectorAdd(exitPosition, XMVector3Rotate(posOffset, rotOffset)));
	viewCamera->GetGameObject()->SetForwardVector(XMVector3Rotate(mainCamera->GetGameObject()->GetForwardVector(), rotOffset));
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
