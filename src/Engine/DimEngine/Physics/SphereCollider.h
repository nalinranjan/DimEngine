#ifndef SPHERE_COLLIDER_H
#define SPHERE_COLLIDER_H

#include "Collider.h"
using namespace DirectX;
using namespace DimEngine;

namespace DimEngine
{
	namespace Physics
	{
		class __declspec(dllexport) SphereCollider : public Collider
		{
		private:
			f32 radius;

		public:
			SphereCollider();
			SphereCollider(f32 radius);
			~SphereCollider();
		};
	}
}
#endif