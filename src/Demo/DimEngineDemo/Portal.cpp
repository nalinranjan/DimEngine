#include "Portal.h"

#include "Core/GameObject.h"

void Portal::Update(f32 deltaTime, f32 gameTime)
{
	XMVECTOR r1 = XMQuaternionInverse(gameObject->GetRotation());
	XMVECTOR r2 = exit->GetGameObject()->GetRotation();

	viewCamera->GetGameObject()->SetForwardVector(XMVector3Rotate(XMVector3Rotate(XMVector3Rotate(mainCamera->GetGameObject()->GetForwardVector(), r1), r2), XMQuaternionRotationRollPitchYaw(0, XM_PI, 0)));
	viewCamera->GetGameObject()->SetLocalPosition(XMVector3Rotate(XMVector3Rotate(XMVectorSubtract(gameObject->GetPosition(), mainCamera->GetGameObject()->GetPosition()), r1), r2));
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
