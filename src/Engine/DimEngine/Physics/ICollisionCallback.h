#ifndef ICOLLISIONCALLBACK_H
#define ICOLLISIONCALLBACK_H
#pragma once


#include "BoundingVolume.h"


namespace DimEngine
{
	class GameObject;

	namespace Physics
	{
		class PhysicsEngine;
	}


	using namespace DimEngine::Physics;


	struct __declspec(dllexport) ICollisionCallback
	{
		friend class PhysicsEngine;


	private:
		virtual void OnBeginOverlapping(GameObject* collider) = 0;
		virtual void OnOverlapping(GameObject* collider) = 0;
		virtual void OnEndOverlapping(GameObject* collider) = 0;
	};
}
#endif