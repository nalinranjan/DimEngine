#pragma once

#include <DirectXMath.h>
#include <vector>

#include "../Portal.h"

#include "Core/GameObject.h"
#include "Rendering/Camera.h"
#include "Rendering/Light.h"
#include "Rendering/Material.h"
#include "Rendering/Mesh.h"
#include "Rendering/SimpleShader.h"
#include "Rendering/Vertex.h"

#include "Core/ResourceManager.h"

#include "DXCore.h"

//using namespace Colliders;
using namespace DimEngine::Rendering;

class Game : public DXCore
{
public:
	Game(HINSTANCE hInstance, char* name);
	~Game();

	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	void OnMouseDown(WPARAM buttonState, int x, int y);
	void OnMouseUp(WPARAM buttonState, int x, int y);
	void OnMouseMove(WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta, int x, int y);


private:
	void LoadShaders();
	void CreateMatrces();
	void CreateBasicGeometry();

	Portal* _create_portal(Material* material, f32 x, f32 y, f32 z, XMVECTOR q);

	POINT prevMousePos;

	SimpleVertexShader* vsZPrepass;
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	SimplePixelShader* psPortal;

	ResourceManager* rm;

	Mesh* sphereMesh;
	Mesh* cubeMesh;

	Material* simpleMaterial;
	Material* portalMaterial1;
	Material* portalMaterial2;

	DirectionalLight* directionalLight;

	GameObject* cameraObject;
	Camera* camera;

	Camera* portalCamera1;
	Camera* portalCamera2;

	Portal* portal1;
	Portal* portal2;

	GameObject* floor;
	GameObject* cube;
	GameObject* sphere;

	ID3D11DepthStencilState* zPrepassDepthStencilState;
};