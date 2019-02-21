#pragma once

#include <DirectXMath.h>

class Transform {

public:

	Transform();

	void SetPosition(float x, float y, float z);
	void SetPosition(DirectX::XMFLOAT3 vec3);
	void SetRotation(float x, float y, float z);
	void SetRotation(DirectX::XMFLOAT3 vec3);
	void SetScale(float x, float y, float z);
	void SetScale(DirectX::XMFLOAT3 vec3);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();
	DirectX::XMFLOAT3 GetScale();

	void Move(float x, float y, float z);
	void Move(DirectX::XMFLOAT3 vec3);
	void Rotate(float x, float y, float z);
	void Rotate(DirectX::XMFLOAT3 vec3);

	DirectX::XMFLOAT3 GetForward();
	DirectX::XMFLOAT3 GetUp();
	DirectX::XMFLOAT3 GetRight();

	DirectX::XMFLOAT4X4 GetWorldMatrix();

private:

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation; // Pitch, Yaw, Roll
	DirectX::XMFLOAT3 scale;

	DirectX::XMFLOAT3 forward;
	DirectX::XMFLOAT3 right;
	DirectX::XMFLOAT3 up;

	DirectX::XMFLOAT4X4 world;

	bool dirtyWorldMatrix;
	void RecalculateWorldMatrix();

	bool dirtyDirectionVectors;
	void RecalculateDirectionVectors();

};