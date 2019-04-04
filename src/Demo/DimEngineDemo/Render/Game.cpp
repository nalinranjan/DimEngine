﻿#include <string>

#include "../CameraController.h"
#include "../Portal.h"

#include "Core/Global.h"
#include "Core/Scene.h"

#include "Rendering/Mesh.h"
#include "Rendering/RenderingEngine.h"
#include "Rendering/Texture.h"

#include "Physics/PhysicsEngine.h"

#include "Game.h"

using namespace DirectX;
using namespace DimEngine;
using namespace DimEngine::Rendering;
using namespace DimEngine::Physics;

Game::Game(HINSTANCE hInstance, char* name) : DXCore(hInstance, name, 1280, 720, true)
{
	vertexShader = nullptr;
	pixelShader = nullptr;

	Global::SetScreenRatio(1280.0f / 720.0f);

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif
}

Game::~Game()
{
	//for (auto& m : entityVector) delete m;

	//delete vertexShader;
	//delete pixelShader;
	//if (mesh) delete mesh;
	//delete camera;
	//if (simpleMaterial) delete simpleMaterial;
}

void Game::Init()
{
	rm = ResourceManager::GetSingleton();
	rm->Initialize(device, context);
	LoadShaders();
	CreateMatrces();
	CreateBasicGeometry();
	DimEngine::Physics::PhysicsEngine::Initialize();
}

void Game::LoadShaders()
{
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	
	std::string spath = std::string(buffer).substr(0, pos).c_str();
	std::wstring wpath = std::wstring(spath.begin(), spath.end());
	std::wstring wVertex = wpath + std::wstring(L"/VertexShader.cso");
	std::wstring wPixel = wpath + std::wstring(L"/PixelShader.cso");
	const wchar_t* vertex = wVertex.c_str();
	const wchar_t* pixel = wPixel.c_str();

	vsZPrepass = new SimpleVertexShader(device, context);
	vsZPrepass->LoadShaderFile((wpath + std::wstring(L"/vs_zprepass.cso")).c_str());
	
	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(vertex);

	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(pixel);

	psPortal = new SimplePixelShader(device, context);
	psPortal->LoadShaderFile((wpath + std::wstring(L"/ps_portal.cso")).c_str());

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	device->CreateDepthStencilState(&depthStencilDesc, &zPrepassDepthStencilState);
}

void Game::CreateMatrces()
{
	//XMMATRIX W = XMMatrixIdentity();
	//XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); 
	//
	//XMVECTOR pos = XMVectorSet(0, 0, -20, 0);
	//XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	//XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	//XMMATRIX V = XMMatrixLookToLH(
	//	pos,     // The position of the "camera"
	//	dir,     // Direction the camera is looking
	//	up);     // "Up" direction in 3D space (prevents roll)
	//XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V));

	//XMMATRIX projection = camera->UpdateProjection((float)width / height);
	//XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(projection)); 
}

void Game::CreateBasicGeometry()
{
	sphereMesh = new Mesh(device, (char*)"../Assets/Models/sphere.obj");
	cubeMesh = new Mesh(device, (char*)"../Assets/Models/cube.obj");

	RenderTexture* portalTexture1 = new RenderTexture(device, 512u);
	RenderTexture* portalTexture2 = new RenderTexture(device, 512u);

	simpleMaterial = new Material(vertexShader, pixelShader, nullptr, nullptr);
	portalMaterial1 = new Material(vertexShader, psPortal, portalTexture1->GetResourceView(), portalTexture1->GetSamplerState());
	portalMaterial2 = new Material(vertexShader, psPortal, portalTexture2->GetResourceView(), portalTexture2->GetSamplerState());
	
	GameObject* directionalLightObject = new GameObject();
	directionalLightObject->SetRotation(90, 0, 0);
	directionalLight = directionalLightObject->AddComponent<DirectionalLight>();


	cameraObject = new GameObject();
	cameraObject->SetLocalRotation(0, 180, 0);
	camera = cameraObject->AddComponent<Camera>();
	cameraObject->AddComponent<CameraController>();
	

	portalCamera1 = (new GameObject())->AddComponent<Camera>();
	portalCamera1->SetRenderTexture(portalTexture1);
	portalCamera1->SetRatio(1);

	portalCamera2 = (new GameObject())->AddComponent<Camera>();
	portalCamera2->SetRenderTexture(portalTexture2);
	portalCamera2->SetRatio(1);

	portal1 = _create_portal(portalMaterial1, 0, 0, -10, XMQuaternionIdentity());
	portal2 = _create_portal(portalMaterial2, 5, 0, 5, XMQuaternionRotationRollPitchYaw(0, XM_1DIV2PI, 0));

	portal1->SetExit(portal2);
	portal1->SetMainCamera(camera);
	portal1->SetViewCamera(portalCamera1);

	portal2->SetExit(portal1);
	portal2->SetMainCamera(camera);
	portal2->SetViewCamera(portalCamera2);


	floor = new GameObject();
	floor->SetPosition(0, -2, 0);
	floor->SetLocalScale(100, 0.1f, 100);
	floor->AddComponent<Renderer>(simpleMaterial, cubeMesh); 
	
	cube = new GameObject();
	cube->SetLocalPosition(0, 0, -15);
	cube->AddComponent<Renderer>(simpleMaterial, cubeMesh);

	sphere = new GameObject();
	sphere->SetLocalPosition(0, 0, 5);
	sphere->AddComponent<Renderer>(simpleMaterial, sphereMesh);
}

__forceinline Portal* Game::_create_portal(Material* material, f32 x, f32 y, f32 z, XMVECTOR q)
{
	GameObject* portal = new GameObject();

	GameObject* portalArea1 = new GameObject();
	portalArea1->SetParent(portal);
	portalArea1->SetLocalRotation(0, 0, 90);
	portalArea1->SetLocalScale(5, 5, 0.1f);
	portalArea1->AddComponent<Renderer>(material, cubeMesh);

	GameObject* pillar1L = new GameObject();
	pillar1L->SetParent(portal);
	pillar1L->SetLocalPosition(-2.5f, 0, 0);
	pillar1L->SetLocalScale(0.2f, 5.5f, 0.2f);
	pillar1L->AddComponent<Renderer>(simpleMaterial, cubeMesh);

	GameObject* pillar1R = new GameObject();
	pillar1R->SetParent(portal);
	pillar1R->SetLocalPosition(2.5f, 0, 0);
	pillar1R->SetLocalScale(0.2f, 5.5f, 0.2f);
	pillar1R->AddComponent<Renderer>(simpleMaterial, cubeMesh);

	portal->SetLocalPosition(x, y, z);
	portal->SetLocalRotation(q);

	return portal->AddComponent<Portal>();
}

void Game::OnResize()
{
	DXCore::OnResize();

	Global::SetScreenRatio((float)width / height);
}

void Game::Update(float deltaTime, float totalTime)
{
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	Scene::GetCurrentScene()->Update(deltaTime, totalTime);

	DimEngine::Physics::PhysicsEngine::GetSingleton()->CollisionsDetection(deltaTime,totalTime);
}

void Game::Draw(float deltaTime, float totalTime)
{
	RenderingEngine* renderingEngine = RenderingEngine::GetSingleton();
	renderingEngine->UpdateRenderables();
	renderingEngine->UpdateViewers();
	renderingEngine->UpdateLightSources();
	renderingEngine->SortRenderables();

	portalCamera1->RenderToRenderTarget(context);
	portalCamera2->RenderToRenderTarget(context);

	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	context->RSSetViewports(1, &viewport);


	const float color[4] = { 0.4f, 1, 1, 0 };

	context->OMSetRenderTargets(1, &backBufferRTV, depthStencilView);
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	
	//renderingEngine->PerformZPrepass(vsZPrepass, context);

	//context->OMSetDepthStencilState(zPrepassDepthStencilState, 0);
	renderingEngine->DrawForward(context);
	//context->OMSetDepthStencilState(nullptr, 0);

	swapChain->Present(0, 0);
}

#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	if (buttonState & 0x0001)
	{
		//camera->SetRotationX((y - prevMousePos.y) * 0.001f);
		//camera->SetRotationY((x - prevMousePos.x) * 0.001f);
	}

	if (buttonState & 0x0002)
		cameraObject->Rotate((y - prevMousePos.y) / 31.41592653579f, (x - prevMousePos.x) / 31.41592653579f, 0.0f);

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion

