#pragma once

#include "Core/GameComponent.h"
#include "TriggerTarget.h"
using namespace DimEngine;

class Trigger : public GameComponent
{
private:
	//float yRot;
	//float xRot;
	bool triggered;
	TriggerTarget * item;
	XMVECTOR defaultPos;
	XMVECTOR targetPos;
	bool turned;
public:
	Trigger(TriggerTarget * _item, XMVECTOR _targetPos);
	void wakeTriggerTarget();

private:
	void Update(f32 deltaTime, f32 totalTime);
	
};
