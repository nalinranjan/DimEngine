#include "../Core/GameObject.h"

#include "Light.h"
#include "RenderingEngine.h"

DimEngine::Light::Light(LightType type) : Light(type, XMVectorSet(0.1f, 0.1f, 0.1f, 1.0f), XMVectorSet(0.6f, 0.6f, 0.0f, 1.0f))
{
}

DimEngine::Light::Light(LightType type, XMVECTOR ambientColor, XMVECTOR diffuseColor)
{
	this->type = type;
	this->ambientColor = ambientColor;
	this->diffuseColor = diffuseColor;

	this->next = nullptr;
	this->previous = nullptr;

	RenderingEngine::GetSingleton()->AddLight(this);
}

DimEngine::Light::~Light()
{
	RenderingEngine::GetSingleton()->RemoveLight(this);
}

XMVECTOR DimEngine::Light::GetPosition()
{
	return gameObject->GetPosition();
}

XMVECTOR DimEngine::Light::GetAmbientColor()
{
	return ambientColor;
}

XMVECTOR DimEngine::Light::GetDiffuseColor()
{
	return diffuseColor;
}

void DimEngine::Light::SetAmbientColor(f32 r, f32 g, f32 b)
{
	ambientColor = XMVectorSet(r, g, b, 0.0f);
}

void DimEngine::Light::SetDiffuseColor(f32 r, f32 g, f32 b)
{
	diffuseColor = XMVectorSet(r, g, b, 0.0f);
}


DimEngine::DirectionalLight::DirectionalLight() : Light(Directional)
{
}

DimEngine::DirectionalLight::DirectionalLight(XMVECTOR ambientColor, XMVECTOR diffuseColor) : Light(Directional, ambientColor, diffuseColor)
{
}

XMVECTOR DimEngine::DirectionalLight::GetDirection()
{
	return XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), gameObject->GetRotation());
}


DimEngine::PointLight::PointLight() : Light(Point)
{
}

DimEngine::PointLight::PointLight(XMVECTOR ambientColor, XMVECTOR diffuseColor) : Light(Point, ambientColor, diffuseColor)
{
}


DimEngine::SpotLight::SpotLight() : Light(Spot)
{
	this->angle = 45.0f;
}

DimEngine::SpotLight::SpotLight(XMVECTOR ambientColor, XMVECTOR diffuseColor, f32 angle) : Light(Spot, ambientColor, diffuseColor)
{
	this->angle = angle;
}
