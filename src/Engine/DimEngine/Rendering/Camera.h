#ifndef CAMERA_H
#define CAMERA_H
#pragma once

#include "../Core/GameComponent.h"

#include "Mesh.h"
#include "Texture.h"
#include "Viewer.h"

using namespace DimEngine::Rendering;


namespace DimEngine
{
	class __declspec(dllexport)  Camera : public GameComponent
	{
		friend class RenderingEngine;


	private:
		f32 fov;
		f32 nearZ;
		f32 farZ;
		i32 viewer;
		Camera* next;
		Camera* previous;


	public:
		Camera();
		~Camera();
	};
}
#endif // !CAMERA_H