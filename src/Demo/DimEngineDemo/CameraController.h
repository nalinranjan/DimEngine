#pragma once

#include "Core/GameComponent.h"

using namespace DimEngine;

class CameraController : public GameComponent
{
private:
	void Update(f32 deltaTime, f32 totalTime);
};