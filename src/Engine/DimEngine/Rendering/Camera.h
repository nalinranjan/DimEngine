#ifndef CAMERA_H
#define CAMERA_H
#pragma once

#include <utility>

#include "../Core/GameComponent.h"

#include "Mesh.h"
#include "Texture.h"
#include "Viewer.h"

using namespace DimEngine::Rendering;

namespace DimEngine
{
	class __declspec(dllexport) Camera : public GameComponent
	{
		friend class RenderingEngine;


	private:
		RenderTexture* renderTexture;

		union
		{
			f32 color[4];
			struct
			{
				f32 r;
				f32 g;
				f32 b;
				f32 a;
			} sColor;
		};

		f32 fov;
		f32 ratio;
		f32 nearZ;
		f32 farZ;
		
		i32 viewer;

		bool useClipPlane;
		//XMFLOAT4 clipPlane;		// Near clip plane in world space
		XMVECTOR clipPlaneNorm;
		XMVECTOR clipPlanePos;

		Camera* next;
		Camera* previous;


	public:
		Camera();
		~Camera();

		void SetRenderTexture(RenderTexture* renderTexture);
		void SetFov(f32 value);
		void SetRatio(f32 value);
		void SetNearZ(f32 value);
		void SetFarZ(f32 value);

		bool UseClipPlane() const;
		//void SetClipPlane(XMFLOAT4 plane, bool usePlane = true);
		void SetClipPlane(XMVECTOR normal, XMVECTOR position, bool usePlane = true);
		//XMFLOAT4 GetClipPlane() const;
		std::pair<XMVECTOR, XMVECTOR> GetClipPlane() const;

		void RenderToRenderTarget(ID3D11DeviceContext* context);
	};
}
#endif // !CAMERA_H