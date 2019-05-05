#include <cstdlib>

#include "../Core/GameObject.h"

#include "Collider.h"
#include "PhysicsEngine.h"

using namespace DirectX;
using namespace DimEngine::Physics;


f32 collisionCheckBack = 0.00f;

bool DimEngine::Collider::IsOverlappingWith(Collider* other)
{
	switch (type)
	{
	case DimEngine::Physics::AABB:
		break;


	case DimEngine::Physics::OBB:
		switch (other->GetType())
		{
		case DimEngine::Physics::AABB:
			break;


		case DimEngine::Physics::OBB:
			return DimEngine::Physics::TestOverlap(static_cast<OrientedBoundingBox*>(GetBoundingVolume()), static_cast<OrientedBoundingBox*>(other->GetBoundingVolume()));
			break;


		case DimEngine::Physics::Sphere:
			return DimEngine::Physics::TestOverlap(static_cast<OrientedBoundingBox*>(GetBoundingVolume()), static_cast<BoundingSphere*>(other->GetBoundingVolume()));
			break;
		}
		break;


	case DimEngine::Physics::Sphere:
		switch (other->GetType())
		{
		case DimEngine::Physics::AABB:
			break;


		case DimEngine::Physics::OBB:
			return DimEngine::Physics::TestOverlap(static_cast<OrientedBoundingBox*>(other->GetBoundingVolume()), static_cast<BoundingSphere*>(GetBoundingVolume()));
			break;


		case DimEngine::Physics::Sphere:
			return DimEngine::Physics::TestOverlap(static_cast<BoundingSphere*>(GetBoundingVolume()), static_cast<BoundingSphere*>(other->GetBoundingVolume()));
			break;
		}
		break;
	}

	throw "NOT IMPLEMENTED";
}

DimEngine::Collider::Collider()
{
	this->next = nullptr;
	this->previous = nullptr;

	this->boundingVolume = nullptr;

	PhysicsEngine::GetSingleton()->AddCollider(this);
}

DimEngine::Collider::Collider(BoundingVolumeType type)
{
	this->next = nullptr;
	this->previous = nullptr;

	this->type = type;
	this->boundingVolume = nullptr;

	PhysicsEngine::GetSingleton()->AddCollider(this);
}

DimEngine::Collider::~Collider()
{
	PhysicsEngine::GetSingleton()->RemoveCollider(this);
}

BoundingVolumeType DimEngine::Collider::GetType()
{
	return type;
}

BoundingVolume* DimEngine::Collider::GetBoundingVolume()
{
	return boundingVolume;
}


DimEngine::BoxCollider::BoxCollider(f32 x, f32 y, f32 z) : BoxCollider(XMVectorSet(x, y, z, 0))
{
}

DimEngine::BoxCollider::BoxCollider(XMVECTOR size, XMVECTOR offset) : Collider(OBB)
{
	this->size = size;
	this->offset = offset;
}

DimEngine::BoxCollider::~BoxCollider()
{
}

__inline XMVECTOR DimEngine::BoxCollider::GetSize() const
{
	return size;
}

__inline XMVECTOR DimEngine::BoxCollider::GetOffset() const
{
	return offset;
}

__inline void DimEngine::BoxCollider::SetSize(f32 x, f32 y, f32 z)
{
	this->size = { x, y, z };
}

__inline void DimEngine::BoxCollider::SetSize(XMVECTOR size)
{
	this->size = size;
}

__inline void DimEngine::BoxCollider::SetOffset(f32 x, f32 y, f32 z)
{
	SetOffset({ x, y, z });
}

void DimEngine::BoxCollider::SetOffset(XMVECTOR xyz)
{
	offset = xyz;
}


DimEngine::SphereCollider::SphereCollider() : SphereCollider(0.5)
{
}

DimEngine::SphereCollider::SphereCollider(f32 radius) : Collider(Sphere)
{
	this->radius = radius;
}

DimEngine::SphereCollider::~SphereCollider()
{
}

__inline f32 DimEngine::SphereCollider::GetRadius() const
{
	return radius;
}

//
//bool isUnique(std::vector<XMVECTOR> points, XMVECTOR test)
//{
//	bool add = true;
//	for (int i = 0; i < points.size(); i++) {
//		XMVECTOR diff = points[i] - test;
//		if (abs(diff.m128_f32[0]) + abs(diff.m128_f32[1]) + abs(diff.m128_f32[2]) < 0.1f) {
//			add = false;
//		}
//	}
//	return add;
//}
//XMVECTOR getPlaneNormal(BoxCollider * Entity, XMVECTOR point)
//{
//	float x = point.m128_f32[0];
//	float y = point.m128_f32[1];
//	float z = point.m128_f32[2];
//	XMVECTOR ret = { 0,0,0 };
//
//	for (int i = 0; i < Entity->planes.size(); i++) {
//		XMVECTOR plane = Entity->planes[i];
//		float A = plane.m128_f32[0];
//		float B = plane.m128_f32[1];
//		float C = plane.m128_f32[2];
//		float D = plane.m128_f32[3];
//		if (abs(A * x + B * y + C * z + D) <= 0.1f) {
//				ret = plane;
//		}
//	}
//
//	/*if (XMVector3Equal(ret, { 0,0,0 })) {
//		printf("6 plane! \n");
//		for (int i = 0; i < Entity->planes.size(); i++) {
//			XMVECTOR plane = Entity->planes[i];
//			float A = plane.m128_f32[0];
//			float B = plane.m128_f32[1];
//			float C = plane.m128_f32[2];
//			float D = plane.m128_f32[3];
//			printf("point:  %f,  %f,  %f \n", point.m128_f32[0], point.m128_f32[1], point.m128_f32[2]);
//			printf("plane:  %f,  %f,  %f \n", plane.m128_f32[0], plane.m128_f32[1], plane.m128_f32[2]);
//			printf("%f \n", abs(A * x + B * y + C * z + D));
//		}
//	}*/
//	return ret;
//}
//XMVECTOR getLineIntersection(XMVECTOR start, XMVECTOR end, std::tuple<XMVECTOR, XMVECTOR, XMVECTOR, XMVECTOR> planeVertetices, 
//	XMFLOAT3 Max, XMFLOAT3 Min)
//{
//	std::vector<XMVECTOR> planeVerteticesVec;
//	planeVerteticesVec.push_back(std::get<0>(planeVertetices));
//	planeVerteticesVec.push_back(std::get<1>(planeVertetices));
//	planeVerteticesVec.push_back(std::get<2>(planeVertetices));
//	planeVerteticesVec.push_back(std::get<3>(planeVertetices));
//	XMVECTOR v1 = end - start;
//	XMVECTOR ip = { 0,0,0 };
//	int counter = 0;
//	for (int i = 0; i < planeVerteticesVec.size()-1; i++)
//	{
//		XMVECTOR A = planeVerteticesVec[i];
//		for (int j = i + 1; j < planeVerteticesVec.size(); j++)
//		{
//			XMVECTOR B = planeVerteticesVec[j];
//			XMVECTOR v2 = XMVector3Normalize(B - A);
//			//printf("v1:  %f,  %f,  %f \n", v1.m128_f32[0], v1.m128_f32[1], v1.m128_f32[2]);
//			//printf("v2:  %f,  %f,  %f \n", v2.m128_f32[0], v2.m128_f32[1], v2.m128_f32[2]);
//			if (abs(XMVector3Dot(v1, v2).m128_f32[0]) == 1)
//			{
//				//printf("line parallel \n ");
//				continue;
//			}
//
//			XMVECTOR diag = XMVector3Normalize(A - start);
//			XMVECTOR vecS1 = XMVector3Cross(v1, v2);            // 有向面积1
//			XMVECTOR vecS2 = XMVector3Cross(diag, v2); // 有向面积2
//			float num = XMVector3Dot(diag, vecS1).m128_f32[0];
//
//			//if (num >= 1E-05f || num <= -1E-05f)
//			//{
//			//	continue;
//			//}
//
//			float num2 = (XMVector3Dot(vecS2, vecS1) / XMVector3LengthSq(vecS1)).m128_f32[0];
//			XMVECTOR point = start + v1 * num2;
//				
//			point = XMVectorClamp(point, { Min.x,Min.y,Min.z }, { Max.x,Max.y,Max.z });
//			ip += point;
//			counter++;
//			//if (point.m128_f32[0] <= Max.x && point.m128_f32[0] >= Min.x &&
//			//	point.m128_f32[1] <= Max.y && point.m128_f32[1] >= Min.y &&
//			//	point.m128_f32[2] <= Max.z && point.m128_f32[2] >= Min.z) {
//			
//			//	printf("add!\n");
//			//}
//			//else {
//			//	printf("out of bound!\n");
//			//}
//			
//		}
//	}
//	if (counter == 0) {
//		return { NAN,NAN,NAN };
//	}
//	XMVECTOR ret = ip / (float)counter;
//	return ret;
//}
