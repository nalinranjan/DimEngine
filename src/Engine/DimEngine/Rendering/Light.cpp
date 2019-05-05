#include "../Core/GameObject.h"

#include "Light.h"
#include "RenderingEngine.h"

DimEngine::Light::Light(LightType type) : Light(type, XMVectorSet(0.1f, 0.1f, 0.1f, 1.0f), XMVectorSet(0.6f, 0.6f, 0.6f, 1.0f))
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


DimEngine::DirectionalLight::DirectionalLight() : Light(Directional)
{
}

DimEngine::DirectionalLight::DirectionalLight(XMVECTOR ambientColor, XMVECTOR diffuseColor) : Light(Directional, ambientColor, diffuseColor)
{
}


DimEngine::PointLight::PointLight() : Light(Point)
{
}

DimEngine::PointLight::PointLight(XMVECTOR ambientColor, XMVECTOR diffuseColor) : Light(Point, ambientColor, diffuseColor)
{
}


DimEngine::SpotLight::SpotLight() : Light(Spot)
{
	this->angle = 30.0f;
}

DimEngine::SpotLight::SpotLight(XMVECTOR ambientColor, XMVECTOR diffuseColor, f32 angle) : Light(Spot, ambientColor, diffuseColor)
{
	this->angle = angle;
}
