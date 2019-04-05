#include "SphereCollider.h"
#include "PhysicsEngine.h"

DimEngine::Physics::SphereCollider::SphereCollider()
{
	radius = 0.5f;
	IsTrigger = false;
	PhysicsEngine::GetSingleton()->AddCollider(this);
}

DimEngine::Physics::SphereCollider::SphereCollider(f32 radius)
{
	this->radius = radius;
	IsTrigger = false;
	PhysicsEngine::GetSingleton()->AddCollider(this);
}


DimEngine::Physics::SphereCollider::~SphereCollider()
{
	PhysicsEngine::GetSingleton()->RemoveCollider(this);
}
