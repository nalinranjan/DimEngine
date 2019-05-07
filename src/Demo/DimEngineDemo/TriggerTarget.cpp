#include "TriggerTarget.h"
#include "Core/GameObject.h"

TriggerTarget::TriggerTarget(XMVECTOR _targetPos)
{
	triggering = false;
	targetPos = _targetPos;
}

void TriggerTarget::Trigger()
{
	if (triggering) {
		XMVECTOR temp = defaultPos;
		defaultPos = targetPos;
		targetPos = temp;
	}
	else {
		defaultPos = gameObject->GetPosition();
		triggering = true;
	}
}

void TriggerTarget::Update(f32 deltaTime, f32 totalTime)
{
	float speed = 3;
	if (triggering) {
		XMVECTOR dir = XMVector3Normalize(targetPos - defaultPos);
		XMVECTOR offset = dir * speed * deltaTime;
		gameObject->SetPosition(XMVectorAdd(gameObject->GetPosition(), offset));
		if (XMVector3Length((targetPos - gameObject->GetPosition())).m128_f32[0] < 0.1f) {
			// the distance to end point is less than 0.1f, end
			gameObject->SetPosition(targetPos);
			XMVECTOR temp = defaultPos;
			defaultPos = targetPos;
			targetPos = temp;
			triggering = false;
		}
	}
}
