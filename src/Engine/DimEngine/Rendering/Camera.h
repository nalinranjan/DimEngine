#ifndef CAMERA_H
#define CAMERA_H
#pragma once

#include "../Core/GameComponent.h"

#include "Mesh.h"
#include "Viewer.h"

using namespace DimEngine::Rendering;

namespace DimEngine
{
	class __declspec(dllexport)  Camera : public GameComponent
	{
		friend class RenderingEngine;


	private:
		ID3D11RenderTargetView* renderTarget;

		f32 fov;
		f32 nearZ;
		f32 farZ;
		
		i32 viewer;

		Camera* next;
		Camera* previous;


	public:
		Camera();
		~Camera();

		void SetRenderTarget(ID3D11RenderTargetView* renderTarget);
		void SetFov(f32 value);
		void SetNearZ(f32 value);
		void SetFarZ(f32 value);

		void RenderToRenderTarget(ID3D11DeviceContext* context, ID3D11DepthStencilView* depthStencilView);
	};
}
#endif // !CAMERA_H