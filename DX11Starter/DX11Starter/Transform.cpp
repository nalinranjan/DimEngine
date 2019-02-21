#pragma once

#include "Transform.h"

using namespace DirectX;

Transform::Transform() {
	position = XMFLOAT3(0, 0, 0);
	scale = XMFLOAT3(1, 1, 1);
	rotation = XMFLOAT3(0, 0, 0);
	dirtyWorldMatrix = true;
}

void Transform::SetPosition(float x, float y, float z) {
	position.x = x;
	position.y = y;
	position.z = z;
	dirtyWorldMatrix = true;
}

void Transform::SetPosition(DirectX::XMFLOAT3 vec3) {
	SetPosition(vec3.x, vec3.y, vec3.z);
}

void Transform::SetRotation(float x, float y, float z) {
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
	dirtyWorldMatrix = true;
	dirtyDirectionVectors = true;
}

void Transform::SetRotation(DirectX::XMFLOAT3 vec3) {
	SetPosition(vec3.x, vec3.y, vec3.z);
}
void Transform::SetScale(float x, float y, float z) {
	scale.x = x;
	scale.y = y;
	scale.z = z;
	dirtyWorldMatrix = true;
}

void Transform::SetScale(DirectX::XMFLOAT3 vec3) {
	SetPosition(vec3.x, vec3.y, vec3.z);
}

XMFLOAT3 Transform::GetPosition() {
	return position;
}

XMFLOAT3 Transform::GetRotation() {
	return rotation;
}

XMFLOAT3 Transform::GetScale() {
	return scale;
}

void Transform::Move(float x, float y, float z) {
	position.x += x;
	position.y += y;
	position.z += z;
	dirtyWorldMatrix = true;
}

void Transform::Move(DirectX::XMFLOAT3 vec3) {
	Move(vec3.x, vec3.y, vec3.z);
}

void Transform::Rotate(float x, float y, float z) {
	rotation.x += x;
	rotation.y += y;
	rotation.z += z;
	dirtyWorldMatrix = true;
	dirtyDirectionVectors = true;
}

void Transform::Rotate(DirectX::XMFLOAT3 vec3) {
	Rotate(rotation.x, rotation.y, rotation.z);
}

XMFLOAT3 Transform::GetForward() {
	if (dirtyDirectionVectors) RecalculateDirectionVectors();
	return forward;
}

XMFLOAT3 Transform::GetUp() {
	if (dirtyDirectionVectors) RecalculateDirectionVectors();
	return up;
}

XMFLOAT3 Transform::GetRight() {
	if (dirtyDirectionVectors) RecalculateDirectionVectors();
	return right;
}

XMFLOAT4X4 Transform::GetWorldMatrix() {
	if (dirtyWorldMatrix) RecalculateWorldMatrix();
	return world;
}

void Transform::RecalculateWorldMatrix() {

	if (!dirtyWorldMatrix) return;

	XMMATRIX tMatrix = XMMatrixTranslation(position.x, position.y, position.z);
	XMMATRIX rMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	XMMATRIX sMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);

	XMMATRIX worldMatrix = sMatrix * rMatrix * tMatrix;
	XMStoreFloat4x4(&world, XMMatrixTranspose(worldMatrix));

	dirtyWorldMatrix = false;

}

void Transform::RecalculateDirectionVectors() {

	if (!dirtyDirectionVectors) return;

	/*
	right = _11, _21, _31
	up = _12, _22, _32
	forward = _13, _23, _33
	*/

	XMVECTOR globalForward = XMVectorSet(0, 0, 1, 0);
	XMVECTOR globalUp = XMVectorSet(0, 1, 0, 0);
	XMVECTOR rotationQuat = XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

	XMVECTOR forwardVec = XMVector3Rotate(globalForward, rotationQuat);
	XMVECTOR upVec = XMVector3Rotate(globalUp, rotationQuat);
	XMVECTOR rightVec = XMVector3Cross(forwardVec, -upVec);

	XMStoreFloat3(&forward, forwardVec);
	XMStoreFloat3(&up, upVec);
	XMStoreFloat3(&right, rightVec);

	dirtyDirectionVectors = false;

}