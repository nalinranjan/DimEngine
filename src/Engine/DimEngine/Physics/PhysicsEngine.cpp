#include "PhysicsEngine.h"

DimEngine::Physics::PhysicsEngine* DimEngine::Physics::PhysicsEngine::singleton = nullptr;

const DimEngine::Physics::PhysicsEngine* DimEngine::Physics::PhysicsEngine::GetSingleton()
{
	if (!singleton)
		Initialize();

	return singleton;
}

void DimEngine::Physics::PhysicsEngine::Initialize()
{
}

DimEngine::Physics::PhysicsEngine::PhysicsEngine()
{
}

void DimEngine::Physics::PhysicsEngine::AddCollision(i32 node1, i32 node2)
{
	{
		Collision* collision = new Collision(node1, node2);

		if (collisionList)
		{
			collision->next = collisionList;
			collisionList->previous = collision;
		}
		else
			collisionList = collision;
	}
}

void DimEngine::Physics::PhysicsEngine::SolveCollision()
{
	for (int i = 0; i < movedObjects.GetSize(); ++i)
		hierarchy.IsOverlappingWith(movedObjects[i], this);
}

bool DimEngine::Physics::PhysicsEngine::DynamicBVHTestOverlapCallback(i32 node1, i32 node2)
{
	if (node1 != node2)
	{
		AddCollision(node1, node2);
	}

	return true;
}
