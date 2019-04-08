#include "PhysicsEngine.h"

DimEngine::Physics::PhysicsEngine* DimEngine::Physics::PhysicsEngine::singleton = nullptr;

DimEngine::Physics::PhysicsEngine* DimEngine::Physics::PhysicsEngine::GetSingleton()
{
	if (!singleton)
		Initialize();

	return singleton;
}

void DimEngine::Physics::PhysicsEngine::Initialize()
{
	singleton = new PhysicsEngine();
}

DimEngine::Physics::PhysicsEngine::PhysicsEngine()
{
	colliderList = nullptr;
}

void DimEngine::Physics::PhysicsEngine::AddCollider(Collider* collider)
{
	collider->next = colliderList;
	collider->previous = nullptr;

	if (colliderList)
		colliderList->previous = collider;

	colliderList = collider;
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

void DimEngine::Physics::PhysicsEngine::RemoveCollider(Collider* collider)
{
	if (!colliderList)
		return;

	Collider* next = collider->next;
	Collider* previous = collider->previous;

	if (previous)
		previous->next = next;

	if (next)
		next = previous;

	if (collider == colliderList)
		colliderList = next;
}

void DimEngine::Physics::PhysicsEngine::SolveCollision()
{
	for (int i = 0; i < movedObjects.GetSize(); ++i)
		hierarchy.IsOverlappingWith(movedObjects[i], this);
}

void DimEngine::Physics::PhysicsEngine::CollisionsDetection(float deltaTime, float totalTime)
{
	for (Collider* a = colliderList; a; a = a->next)
	{
		//collider->ApplyGravity(deltaTime);
		if (a->next == nullptr) continue;
		for (Collider* b = a ->next; b; b = b->next)
		{
			// in the future, if the collider belongs to the subObject of current checking one, it should has the option to ignore it.
			if (a != b){

				bool collied = a->IsOverlappingWith(b, totalTime);
				if (collied) {
					
					a->LogCollision(b, totalTime);
					b->LogCollision(a, totalTime);
					if (a->IsTrigger || b->IsTrigger) {
						// Trigger the collision event
						a->gameObject->IsTriggerEnter(b->gameObject);
						b->gameObject->IsTriggerEnter(a->gameObject);
					}
					else {
						// Trigger the collision event
						a->gameObject->IsCollisionEnter(b->gameObject);
						b->gameObject->IsCollisionEnter(a->gameObject);


						// Push back the object
						a->PreventOverlaps();
						b->PreventOverlaps();
					}
				}
				else if (a->CollidedWith[b] != 0 && !collied) {
					a->CollidedWith[b] = 0.0f;
					b->CollidedWith[a] = 0.0f;

					if (a->IsTrigger || b->IsTrigger) {
						a->gameObject->IsTriggerExit(b->gameObject);
						b->gameObject->IsTriggerExit(a->gameObject);
					}
					else {
						a->gameObject->IsCollisionExit(b->gameObject);
						b->gameObject->IsCollisionExit(a->gameObject);
					}
				}
			}
		}
		a->Update(deltaTime,totalTime);
	}
}


bool DimEngine::Physics::PhysicsEngine::DynamicBVHTestOverlapCallback(i32 node1, i32 node2)
{
	if (node1 != node2)
	{
		AddCollision(node1, node2);
	}

	return true;
}
