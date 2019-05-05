#ifndef RENDERABLE_H
#define RENDERABLE_H
#pragma once

#include "Material.h"
#include "Mesh.h"

namespace DimEngine
{
	namespace Rendering
	{
		struct Renderable
		{
			friend class RenderingEngine;


		private:
			Mesh* mesh;
			Material* material;

			XMMATRIX worldMatrix;

			Renderable();
		};
	}
}
#endif