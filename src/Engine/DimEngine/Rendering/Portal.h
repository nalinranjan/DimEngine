#pragma once

#include <DirectXMath.h>

#include "../Core/GameComponent.h"

#include "Camera.h"

using namespace DimEngine;
using namespace DirectX;

class __declspec(dllexport) Portal : public GameComponent
{
private:
	Camera* mainCamera;
	Camera* viewCamera;
	Portal* exit;

	void LateUpdate(f32 deltaTime, f32 gameTime);


public:

	struct Warp {
		Warp(const Portal* fromPortal) : fromPortal(fromPortal), toPortal(nullptr) {
			XMStoreFloat4x4(&delta, XMMatrixIdentity());
			XMStoreFloat4x4(&deltaInv, XMMatrixIdentity());
		}
		XMFLOAT4X4 delta;
		XMFLOAT4X4 deltaInv;
		const Portal* fromPortal;
		const Portal* toPortal;
	};

	Portal* GetExit();
	Camera* GetViewCamera();

	void SetMainCamera(Camera* camera);
	void SetViewCamera(Camera* camera);
	void SetExit(Portal* portal);
	void SetViewClipPlane();
	
	//static void Connect(Portal* a, Portal* b);
	//static void Connect(Warp& a, Warp& b);

};
