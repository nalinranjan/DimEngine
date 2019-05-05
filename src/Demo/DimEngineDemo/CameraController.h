#pragma once

#include "Core/GameComponent.h"

using namespace DimEngine;

class CameraController : public GameComponent
{
private:
	//float yRot;
	//float xRot;

	GameObject* exitPortal;
	XMVECTOR previousPos;
	bool colliedWithWall;

	GameObject* trigger;

public:
	CameraController();


private:
	void Update(f32 deltaTime, f32 totalTime);

	void OnBeginOverlapping(GameObject* other);
	void OnOverlapping(GameObject* other);
	void OnEndOverlapping(GameObject* other);
};