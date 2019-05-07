#include "Core/GameObject.h"

#include "CameraController.h"
#include "Portal.h"
#include "Trigger.h"


CameraController::CameraController()
{
	colliedWithWall = false;
	exitPortal = nullptr;
	trigger = nullptr;
}
#include <iostream>
void CameraController::Update(f32 deltaTime, f32 totalTime)
{
	float speed = 6;
	float forwardScale = 0;
	float rightScale = 0;

	if (GetAsyncKeyState('W') & 0x8000)
		forwardScale = 1;

	if (GetAsyncKeyState('A') & 0x8000)
		rightScale = -1;

	if (GetAsyncKeyState('S') & 0x8000)
		forwardScale = -1;

	if (GetAsyncKeyState('D') & 0x8000)
		rightScale = 1;

	XMVECTOR forward = gameObject->GetForwardVector();
	XMVECTOR flatForward = XMVector3Normalize(XMVectorMultiply(forward, XMVectorSet(1, 0, 1, 1)));

	XMVECTOR right = gameObject->GetRightVector();
	XMVECTOR flatRight = XMVector3Normalize(XMVectorMultiply(right, XMVectorSet(1, 0, 1, 1)));
	previousPos = gameObject->GetPosition();
	if (!colliedWithWall) {
		XMVECTOR forwardOffset = flatForward * speed * forwardScale * deltaTime;
		XMVECTOR rightOffset = flatRight * speed * rightScale * deltaTime;
		gameObject->SetPosition(XMVectorAdd(gameObject->GetPosition(), XMVectorAdd(forwardOffset, rightOffset)));
	}

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 && trigger) {
		trigger->GetComponent<Trigger>()->wakeTriggerTarget();
	}
}

void CameraController::OnBeginOverlapping(GameObject* other)
{
	if (other->ContainTag("Portal") && other->GetParent() != exitPortal)
	{
		exitPortal = other->GetParent()->GetComponent<Portal>()->GetExit()->GetGameObject();
		XMVECTOR offset = XMVectorSubtract(GetGameObject()->GetPosition(), other->GetPosition());
		gameObject->SetPosition(XMVectorAdd(exitPortal->GetPosition(), offset));
		//gameObject->SetForwardVector(exitPortal->GetForwardVector());
	}
	else {
		if (other->ContainTag("Wall")) {
			gameObject->SetPosition(previousPos);
			colliedWithWall = true;
		}

		if (other->ContainTag("TriggerBox")) {
			trigger = other;
		}
	
	}


}

void CameraController::OnOverlapping(GameObject * other)
{
	
}

void CameraController::OnEndOverlapping(GameObject* other)
{
	if (other->ContainTag("Portal") && other->GetParent() == exitPortal)
		exitPortal = nullptr;

	else {
		if (other->ContainTag("Wall"))
			colliedWithWall = false;

		if (other->ContainTag("TriggerBox"))
			trigger = nullptr;
	}
}
