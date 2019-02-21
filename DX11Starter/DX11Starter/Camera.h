#pragma once

#include "Transform.h"
#include "InputManager.h"

class Camera {

public:

	Camera(bool _grounded);
	~Camera();

	void Update(float _deltaTime);
	void Rotate(float _h, float _v);
	void SetProjection(float _aspectRatio);

	Transform* GetTransform();
	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjectionMatrix();

private:

	Input* input = Input::Instance();

	const float moveSpeed = 3.f;
	const float moveSpeedMultiplier = 3.f;
	const float rotationSpeed = 0.002f;

	bool grounded = false;
	float horizontalRotation = 0;
	float verticalRotation = 0;

	Transform* transform;

	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;

	void RecalculateMatrices();

};