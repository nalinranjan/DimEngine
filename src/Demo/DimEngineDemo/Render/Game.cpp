#include <string>

#include "../CameraController.h"
#include "../Portal.h"

#include "Core/Global.h"
#include "Core/Scene.h"

#include "Physics/PhysicsEngine.h"

#include "Rendering/Mesh.h"
#include "Rendering/RenderingEngine.h"
#include "Rendering/Texture.h"

#include "Game.h"

using namespace DirectX;

using namespace DimEngine;
using namespace DimEngine::Physics;
using namespace DimEngine::Rendering;


Game::Game(HINSTANCE hInstance, char* name) : DXCore(hInstance, name, 1280, 720, true)
{
	vsZPrepass = nullptr;
	vertexShader = nullptr;
	pixelShader = nullptr;
	psPortal = nullptr;


	sphereMesh = nullptr;
	cubeMesh = nullptr;


	grassTexture = nullptr;
	wallTexture = nullptr;
	rockTexture = nullptr;
	
	portalTexture1 = nullptr;
	portalTexture2 = nullptr;


	grassMaterial = nullptr;
	wallMaterial = nullptr;
	rockMaterial = nullptr;
	
	portalMaterial1 = nullptr;
	portalMaterial2 = nullptr;


	directionalLight = nullptr;

	cameraObject = nullptr;
	camera = nullptr;

	portalCamera1 = nullptr;
	portalCamera2 = nullptr;

	portal1 = nullptr;
	portal2 = nullptr;

	floor = nullptr;
	cube = nullptr;


	zPrepassDepthStencilState = nullptr;


	Global::SetScreenRatio(1280.0f / 720.0f);


#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif
}

Game::~Game()
{
	if (vsZPrepass)
		delete vsZPrepass;

	if (vertexShader)
		delete vertexShader;

	if (pixelShader)
		delete pixelShader;

	if (psPortal)
		delete psPortal;


	if (sphereMesh)
		delete sphereMesh;

	if (cubeMesh)
		delete cubeMesh;

	if (grassTexture)
		delete grassTexture;

	if (wallTexture)
		delete wallTexture;

	if (rockTexture)
		delete rockTexture;

	if (portalTexture1)
		delete portalTexture1;

	if (portalTexture2)
		delete portalTexture2;


	if (grassMaterial)
		delete grassMaterial;

	if (wallMaterial)
		delete wallMaterial;

	if (rockMaterial)
		delete rockMaterial;

	if (portalMaterial1)
		delete portalMaterial1;

	if (portalMaterial2)
		delete portalMaterial2;


	if (zPrepassDepthStencilState)
		zPrepassDepthStencilState->Release();


	Scene::UnloadAll();

	RenderingEngine::Stop();
}

void Game::Init()
{
	PhysicsEngine::Initialize();
	RenderingEngine::Initialize();

	rm = ResourceManager::GetSingleton();
	rm->Initialize(device, context);
	
	LoadShaders();
	CreateScene();
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

	vsPortal = new SimpleVertexShader(device, context);
	vsPortal->LoadShaderFile((wpath + std::wstring(L"/vs_portal.cso")).c_str());

	psPortal = new SimplePixelShader(device, context);
	psPortal->LoadShaderFile((wpath + std::wstring(L"/ps_portal.cso")).c_str());


	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	device->CreateDepthStencilState(&depthStencilDesc, &zPrepassDepthStencilState);
}

void Game::CreateScene()
{
	sphereMesh = new Mesh(device, (char*)"../Assets/Models/sphere.obj");
	cubeMesh = new Mesh(device, (char*)"../Assets/Models/cube.obj");
	floorMesh = new Mesh(device, (char*)"../Assets/Models/floor.obj");
	quadMesh = new Mesh(device, (char*)"../Assets/Models/quad.obj");
	coneMesh = new Mesh(device, (char*)"../Assets/Models/cone.obj");
	tunnelMesh = new Mesh(device, (char*)"../Assets/Models/tunnel.obj");

	grassTexture = new Texture((wchar_t*)L"../Assets/Textures/checkered-ground.png", D3D11_TEXTURE_ADDRESS_WRAP, D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_FLOAT32_MAX, device, context);
	wallTexture = new Texture((wchar_t*)L"../Assets/Textures/wall.jpg", D3D11_TEXTURE_ADDRESS_WRAP, D3D11_FILTER_ANISOTROPIC, D3D11_FLOAT32_MAX, device, context);
	rockTexture = new Texture((wchar_t*)L"../Assets/Textures/rock.jpg", D3D11_TEXTURE_ADDRESS_WRAP, D3D11_FILTER_ANISOTROPIC, D3D11_FLOAT32_MAX, device, context);
	portalTexture1 = new RenderTexture(device, 1280u, 720u);
	portalTexture2 = new RenderTexture(device, 1280u, 720u);

	grassMaterial = new Material(vertexShader, pixelShader, grassTexture->GetResourceView(), grassTexture->GetSamplerState());
	wallMaterial = new Material(vertexShader, pixelShader, wallTexture->GetResourceView(), wallTexture->GetSamplerState());
	rockMaterial = new Material(vertexShader, pixelShader, rockTexture->GetResourceView(), rockTexture->GetSamplerState());
	portalMaterial1 = new Material(vsPortal, psPortal, portalTexture1->GetResourceView(), portalTexture1->GetSamplerState());
	portalMaterial2 = new Material(vsPortal, psPortal, portalTexture2->GetResourceView(), portalTexture2->GetSamplerState());
	
	GameObject* directionalLightObject = new GameObject();
	directionalLightObject->SetRotation(45, 0, 0);
	directionalLight = directionalLightObject->AddComponent<DirectionalLight>();


	cameraObject = new GameObject();
	cameraObject->SetRotation(camRotX, camRotY, 0);
	//cameraObject->AddComponent<Renderer>(rockMaterial, sphereMesh);
	cameraObject->AddComponent<SphereCollider>();
	cameraObject->AddComponent<CameraController>();
	cameraObject->SetPosition(0, 0, 0);

	GameObject* go = new GameObject();
	go->SetParent(cameraObject);
	go->SetLocalRotation(45, 0, 0);
	go->Translate(0, 0, 0);
	camera = go->AddComponent<Camera>();
	

	portalCamera1 = (new GameObject())->AddComponent<Camera>();
	portalCamera1->SetRenderTexture(portalTexture1);
	portalCamera1->SetRatio((float)width / height);

	portalCamera2 = (new GameObject())->AddComponent<Camera>();
	portalCamera2->SetRenderTexture(portalTexture2);
	portalCamera2->SetRatio((float)width / height);

	portal1 = __CreatePortal(portalMaterial1, -10, 0, 10, 0, 270, 0);
	portal2 = __CreatePortal(portalMaterial2, 10, 0, 10, 0, 90, 0);

	portal1->SetExit(portal2);
	portal1->SetMainCamera(camera);
	portal1->SetViewCamera(portalCamera1);

	portal2->SetExit(portal1);
	portal2->SetMainCamera(camera);
	portal2->SetViewCamera(portalCamera2);

	portalCamera1->GetGameObject()->SetParent(portal2->GetGameObject());
	portalCamera1->GetGameObject()->SetLocalPosition(0, 0, 0);
	portalCamera2->GetGameObject()->SetParent(portal1->GetGameObject());
	portalCamera2->GetGameObject()->SetLocalPosition(0, 0, 0);


	floor = new GameObject();
	floor->SetPosition(0, -2, 0);
	floor->SetLocalScale(100, 100, 1);
	floor->SetRotation(-90, 0, 0);
	floor->AddComponent<Renderer>(grassMaterial, floorMesh);
	
	/*tunnel1 = new GameObject();
	tunnel1->SetPosition(-10, -2, 10);
	tunnel1->SetLocalScale(2, 2, 4);
	tunnel1->AddComponent<Renderer>(rockMaterial, tunnelMesh);

	tunnel2 = new GameObject();
	tunnel2->SetPosition(10, -2, 10);
	tunnel2->SetLocalScale(2, 2, 8);
	tunnel2->AddComponent<Renderer>(rockMaterial, tunnelMesh);*/

	cube = new GameObject();
	cube->SetParent(portal2->GetGameObject());
	cube->SetLocalPosition(0, 0, 5);
	cube->AddComponent<Renderer>(rockMaterial, cubeMesh);

}

__forceinline Portal* Game::__CreatePortal(Material* material, f32 x, f32 y, f32 z, f32 rx, f32 ry, f32 rz)
{
	GameObject* portal = new GameObject();

	GameObject* portalArea1 = new GameObject();
	portalArea1->SetParent(portal);
	portalArea1->SetLocalRotation(0, 0, 90);
	portalArea1->SetLocalScale(2.5f, 2.5f, 1);
	portalArea1->AddComponent<Renderer>(material, quadMesh);
	portalArea1->AddComponent<BoxCollider>(2, 2, 0.1f);

	GameObject* pillar1L = new GameObject();
	pillar1L->SetParent(portal);
	pillar1L->SetLocalPosition(-2.5f, 0, 0);
	pillar1L->SetLocalScale(0.2f, 5, 0.2f);
	pillar1L->AddComponent<Renderer>(wallMaterial, cubeMesh);

	GameObject* pillar1R = new GameObject();
	pillar1R->SetParent(portal);
	pillar1R->SetLocalPosition(2.5f, 0, 0);
	pillar1R->SetLocalScale(0.2f, 5, 0.2f);
	pillar1R->AddComponent<Renderer>(wallMaterial, cubeMesh);

	portal->SetLocalPosition(x, y, z);
	portal->SetLocalRotation(rx, ry, rz);

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

	PhysicsEngine* physicsEngine = PhysicsEngine::GetSingleton();
	physicsEngine->UpdateBoundingVolumes();
	physicsEngine->SolveCollisions();
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


	const float color[4] = { 0.69f, 0.88f, 0.9f, 0.0f };

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

	if (buttonState & 0x0002) {
		camRotX += (float)(y - prevMousePos.y) / 5.0f;
		camRotY += (float)(x - prevMousePos.x) / 5.0f;
		cameraObject->SetRotation(camRotX, camRotY, 0);
	}

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

