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
	void CreateScene();

	Portal* __CreatePortal(Material* material, f32 x = 0, f32 y = 0, f32 z = 0, f32 rx = 0, f32 ry = 0, f32 rz = 0);

	ResourceManager* rm;

	POINT prevMousePos;

	SimpleVertexShader* vsZPrepass;
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	SimpleVertexShader* vsPortal;
	SimplePixelShader* psPortal;


	Mesh* sphereMesh;
	Mesh* cubeMesh;
	Mesh* quadMesh;

	Texture* grassTexture;
	Texture* wallTexture;
	Texture* rockTexture;
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
	f32 camRotX = 0;
	f32 camRotY = 0;

	Camera* portalCamera1;
	Camera* portalCamera2;

	Portal* portal1;
	Portal* portal2;

	GameObject* floor;
	GameObject* cube;
	GameObject* sphere;


	ID3D11DepthStencilState* zPrepassDepthStencilState;

	ID3D11DepthStencilState* portalPass1DepthStencilState;
	ID3D11DepthStencilState* portalPass2DepthStencilState;
};