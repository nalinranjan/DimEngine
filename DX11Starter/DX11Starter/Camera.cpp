#include "Camera.h"

#include "InputManager.h"
#include <Windows.h>
#include <iostream>

using namespace DirectX;

Camera::Camera(bool _grounded) {
	transform = new Transform();
	grounded = _grounded;
}

Camera::~Camera() {
	delete transform;
}

void Camera::Update(float _deltaTime) {

	bool shift = Input::Instance()->GetKey(VK_LSHIFT);
	float finalSpeed = moveSpeed * (shift ? moveSpeedMultiplier : 1) * _deltaTime;

	// Move forward/backward
	bool w = Input::Instance()->GetKey('W');
	bool s = Input::Instance()->GetKey('S');
	if (w ^ s) {

		float dir = w ? 1 : -1;

		XMVECTOR f = XMLoadFloat3(&(transform->GetForward()));
		if (grounded) f = XMVectorSetY(f, 0);
		f = XMVectorScale(f, dir * finalSpeed);

		XMFLOAT3 deltaForward;
		XMStoreFloat3(&deltaForward, f);

		transform->Move(deltaForward);

	}

	// Move left/right
	bool a = Input::Instance()->GetKey('A');
	bool d = Input::Instance()->GetKey('D');
	if (a ^ d) {

		float dir = d ? 1 : -1;

		XMVECTOR r = XMLoadFloat3(&(transform->GetRight()));
		if (grounded) r = XMVectorSetY(r, 0);
		r = XMVectorScale(r, dir * finalSpeed);

		XMFLOAT3 deltaRight;
		XMStoreFloat3(&deltaRight, r);

		transform->Move(deltaRight);

	}

	// Move up/down
	if (!grounded) {
		bool space = Input::Instance()->GetKey(' ');
		bool x = Input::Instance()->GetKey('X');
		if (space ^ x) {

			float dir = space ? 1 : -1;

			XMVECTOR f = XMLoadFloat3(&(transform->GetUp()));
			f = XMVectorScale(f, dir * finalSpeed);

			XMFLOAT3 deltaUp;
			XMStoreFloat3(&deltaUp, f);

			transform->Move(deltaUp);

		}
	}

	// Rotate
	if (input->GetMouse(2)) {
		XMFLOAT2 mouseDelta = input->GetMouseDelta();
		Rotate(mouseDelta.x, mouseDelta.y);
		transform->SetRotation(verticalRotation, horizontalRotation, 0);
	}

}

void Camera::Rotate(float _h, float _v) {
	horizontalRotation += _h * rotationSpeed;
	verticalRotation += _v * rotationSpeed;
}

void Camera::SetProjection(float _aspectRatio) {
	XMMATRIX projMatrix = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		_aspectRatio,			// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&projection, XMMatrixTranspose(projMatrix)); // Transpose for HLSL!
}

Transform* Camera::GetTransform() {
	return transform;
}

XMFLOAT4X4 Camera::GetViewMatrix() {
	RecalculateMatrices();
	return view;
}

XMFLOAT4X4 Camera::GetProjectionMatrix() {
	return projection;
}

void Camera::RecalculateMatrices() {

	XMVECTOR position = XMLoadFloat3(&(transform->GetPosition()));
	XMVECTOR forward = XMLoadFloat3(&(transform->GetForward()));
	XMVECTOR up = XMLoadFloat3(&(transform->GetUp()));
	XMMATRIX viewMatrix = XMMatrixLookToLH(position, forward, up);
	XMStoreFloat4x4(&view, XMMatrixTranspose(viewMatrix));

}