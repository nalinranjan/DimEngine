#include "Trigger.h"
#include "Core/GameObject.h"

Trigger::Trigger(TriggerTarget * _item, XMVECTOR _targetPos)
{
	turned = false;
	item = _item;
	targetPos = _targetPos;
	triggered = false;
}

void Trigger::wakeTriggerTarget()
{
	if (!triggered) {
		defaultPos = gameObject->GetPosition();
		triggered = true;
		turned = false;
		item->Trigger();	
	}
}

void Trigger::Update(f32 deltaTime, f32 totalTime)
{
	float speed = 3;
	if (triggered) {
		XMVECTOR dir = XMVector3Normalize(targetPos - defaultPos);
		XMVECTOR offset = dir * speed * deltaTime;
		gameObject->SetPosition(XMVectorAdd(gameObject->GetPosition(), offset));
		if (XMVector3Length((targetPos - gameObject->GetPosition())).m128_f32[0] < 0.1f ) {
			// the distance to end point is less than 0.1f, end
			gameObject->SetPosition(targetPos);
			XMVECTOR temp = defaultPos;
			defaultPos = targetPos;
			targetPos = temp;
			if (!turned) {
				turned = true;
			}else{
				triggered = false;
			}
		}
	}
}
