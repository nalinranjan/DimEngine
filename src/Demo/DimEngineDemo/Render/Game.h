#pragma once

#include <DirectXMath.h>
#include <vector>

#include "Core/GameObject.h"
#include "Rendering/Camera.h"
#include "Rendering/Light.h"
#include "Rendering/Material.h"
#include "Rendering/Mesh.h"
#include "Rendering/SimpleShader.h"
#include "Rendering/Vertex.h"

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

	POINT prevMousePos;

	SimpleVertexShader* vsZPrepass;
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	SimplePixelShader* psPortal;

	//XMFLOAT4X4 worldMatrix;
	//XMFLOAT4X4 viewMatrix;
	//XMFLOAT4X4 projectionMatrix;

	Mesh* sphereMesh;
	Mesh* cubeMesh;

	//CollisionManager* physics;

	//std::vector<Entity*> entityVector;

	Material* simpleMaterial;
	Material* portalMaterial;

	GameObject* camera;
	DirectionalLight* directionalLight;
	Camera* portalCamera;

	GameObject* floor;
	GameObject* portal;

	GameObject* cube;
	GameObject* sphere;

	ID3D11DepthStencilState* zPrepassDepthStencilState;

	ID3D11SamplerState* portalSampler;

	ID3D11DepthStencilView* portalDSV;
	ID3D11ShaderResourceView* portalRSV;
	ID3D11RenderTargetView* portalRTV;

	//PointLight pointLight;
};