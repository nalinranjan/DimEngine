#pragma once

#include "Core/GameComponent.h"

using namespace DimEngine;

class TriggerTarget : public GameComponent
{
private:
	XMVECTOR defaultPos;
	XMVECTOR targetPos;
	bool triggering;

public:
	TriggerTarget(XMVECTOR _targetPos);
	void Trigger();

private:
	void Update(f32 deltaTime, f32 totalTime);
}; 
