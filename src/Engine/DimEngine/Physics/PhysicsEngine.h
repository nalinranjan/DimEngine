#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H


#include "Collider.h"
//#include "Collision.h"
//#include "DynamicBVH.hpp"

using namespace DimEngine;
using namespace std;


namespace DimEngine
{
	namespace Physics
	{
		class __declspec(dllexport) PhysicsEngine
		{
			friend class Collider;


		public:
			static PhysicsEngine* singleton;

			static PhysicsEngine* GetSingleton();
			static void Initialize();


		private:
			Collider* colliderList;


			PhysicsEngine();


			void AddCollider(Collider* collider);

			void RemoveCollider(Collider* collider);


		public:
			void UpdateBoundingVolumes();
			void SolveCollisions();
		};
	}
}
#endif