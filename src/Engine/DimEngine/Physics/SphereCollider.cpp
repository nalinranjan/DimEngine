#include "SphereCollider.h"
#include "PhysicsEngine.h"

DimEngine::Physics::SphereCollider::SphereCollider()
{
	radius = 0.5f;
	IsTrigger = false;
	type = DimEngine::Physics::Sphere;
	boundingVolume = new BoundingSphere(gameObject->GetPosition(), radius);
	PhysicsEngine::GetSingleton()->AddCollider(this);
}

DimEngine::Physics::SphereCollider::SphereCollider(f32 radius)
{
	this->radius = radius;
	IsTrigger = false;
	type = DimEngine::Physics::Sphere;
	boundingVolume = new BoundingSphere(gameObject->GetPosition(), radius);
	PhysicsEngine::GetSingleton()->AddCollider(this);
}


DimEngine::Physics::SphereCollider::~SphereCollider()
{
	PhysicsEngine::GetSingleton()->RemoveCollider(this);
}
