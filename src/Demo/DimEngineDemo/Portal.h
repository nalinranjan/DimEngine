#pragma once

#include <DirectXMath.h>

#include "Core/GameComponent.h"

#include "Rendering/Camera.h"

using namespace DimEngine;
using namespace DirectX;

class __declspec(dllexport) Portal : public GameComponent
{
private:
	Camera* mainCamera;
	Camera* viewCamera;
	Portal* exit;

	void Update(f32 deltaTime, f32 gameTime);


public:
	void SetMainCamera(Camera* camera);
	void SetViewCamera(Camera* camera);
	void SetExit(Portal* portal);
};
