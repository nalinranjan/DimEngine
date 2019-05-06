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
#include "Rendering/ShadowMap.h"

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
	Mesh* floorMesh;
	Mesh* quadMesh;
	Mesh* tunnelMesh;
	Mesh* coneMesh;

	Texture* grassTexture;
	Texture* wallTexture;
	Texture* rockTexture;
	RenderTexture* portalTexture1;
	RenderTexture* portalTexture2;
	RenderTexture* portalTexture3;
	RenderTexture* portalTexture4;

	Material* grassMaterial;
	Material* wallMaterial;
	Material* rockMaterial;
	Material* portalMaterial1;
	Material* portalMaterial2;
	Material* portalMaterial3;
	Material* portalMaterial4;


	DirectionalLight* directionalLight;

	GameObject* cameraObject;
	Camera* camera;
	f32 camRotX = 0;
	f32 camRotY = 0;

	Camera* portalCamera1;
	Camera* portalCamera2;
	Camera* portalCamera3;
	Camera* portalCamera4;

	Portal* portal1;
	Portal* portal2;
	Portal* portal3;
	Portal* portal4;

	GameObject* floor;
	GameObject* cube;
	GameObject* tunnel1;
	GameObject* tunnel2;

	ID3D11ShaderResourceView* texture;
	ID3D11ShaderResourceView* normalMap;

	ID3D11SamplerState* sampler;
	D3D11_SAMPLER_DESC samplerDesc;

	ShadowMap* shadow;

	SimpleVertexShader* shadowShader;

	D3D11_VIEWPORT shadowViewport;
	float shadowMapSize;

	ID3D11ShaderResourceView* metalnessMap;
	ID3D11ShaderResourceView* roughnessMap;

	SimplePixelShader* psPBR;




	ID3D11DepthStencilState* zPrepassDepthStencilState;
};