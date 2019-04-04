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

	Portal* _create_portal(Material* material, f32 x = 0, f32 y = 0, f32 z = 0, f32 rx = 0, f32 ry = 0, f32 rz = 0);

	ResourceManager* rm;

	POINT prevMousePos;

	SimpleVertexShader* vsZPrepass;
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	SimplePixelShader* psPortal;

	Texture* grassTexture;
	Texture* wallTexture;
	Texture* rockTexture;

	Mesh* sphereMesh;
	Mesh* cubeMesh;

	RenderTexture* portalTexture1;
	RenderTexture* portalTexture2;

	Material* grassMaterial;
	Material* wallMaterial;
	Material* rockMaterial;

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