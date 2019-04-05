#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include <vector>

#include "../Common/Stack.hpp"

#include "Collider.h"
#include "Collision.h"
#include "DynamicBVH.hpp"

using namespace std;

namespace DimEngine
{
	namespace Physics
	{
		class __declspec(dllexport) PhysicsEngine : IDynamicBVHQueryCallback
		{
			friend class Collider;


		public:
			static PhysicsEngine* singleton;
			static void Initialize();
			static PhysicsEngine* GetSingleton();
			void CollisionsDetection(float deltaTime, float totalTime);
			void AddCollider(Collider* collider);
			void RemoveCollider(Collider* collider);
			void AddCollision(i32 node1, i32 node2);

		private:
			

			DynamicBVH<Collider> hierarchy;
			Stack<int> movedObjects;
			Collider* colliderList;
			Collision* collisionList;

			PhysicsEngine();

			




			void SolveCollision();
			
			bool DynamicBVHTestOverlapCallback(i32 node1, i32 node2);
		};
	}
}
#endif