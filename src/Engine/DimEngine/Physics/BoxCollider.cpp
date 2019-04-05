#include "BoxCollider.h"

DimEngine::Physics::BoxCollider::BoxCollider()
{
	size = { 1,1,1 };
}

DimEngine::Physics::BoxCollider::BoxCollider(XMVECTOR size)
{
	this->size = size;
}

DimEngine::Physics::BoxCollider::~BoxCollider()
{
}
