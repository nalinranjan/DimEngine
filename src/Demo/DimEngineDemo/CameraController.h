#pragma once

#include "Core/GameComponent.h"

using namespace DimEngine;


class __declspec(dllexport) CameraController : public GameComponent
{
private:
	//float yRot;
	//float xRot;

	GameObject* exitPortal;


public:
	CameraController();


private:
	void Update(f32 deltaTime, f32 totalTime);

	void OnBeginOverlapping(GameObject* other);
	void OnEndOverlapping(GameObject* other);
};