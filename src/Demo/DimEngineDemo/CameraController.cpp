#include "Core/GameObject.h"

#include "CameraController.h"

void CameraController::Update(f32 deltaTime, f32 totalTime)
{
	if (GetAsyncKeyState('W') & 0x8000)
		gameObject->Translate(0.0f, 0.0f, 5 * deltaTime, SELF);

	if (GetAsyncKeyState('A') & 0x8000)
		gameObject->Translate(-5 * deltaTime, 0.0f, 0.0f, SELF);

	if (GetAsyncKeyState('S') & 0x8000)
		gameObject->Translate(0.0f, 0.0f, -5 * deltaTime, SELF);

	if (GetAsyncKeyState('D') & 0x8000)
		gameObject->Translate(5 * deltaTime, 0.0f, 0.0f, SELF);

	//if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	//	gameObject->Translate(0.0f, deltaTime, 0.0f, SELF);

	//if (GetAsyncKeyState('X') & 0x8000)
	//	gameObject->Translate(0.0f, -deltaTime, 0.0f, SELF);
}