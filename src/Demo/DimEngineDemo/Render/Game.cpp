#include <string>

#include "../CameraController.h"


#include "../Trigger.h"
#include "../TriggerTarget.h"

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
	
	//portalTexture1 = nullptr;
	//portalTexture2 = nullptr;


	grassMaterial = nullptr;
	wallMaterial = nullptr;
	rockMaterial = nullptr;
	
	//portalMaterial1 = nullptr;
	//portalMaterial2 = nullptr;
	//portalMaterial3 = nullptr;
	//portalMaterial4 = nullptr;
	portalMaterial = nullptr;


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

	texture = nullptr;
	normalMap = nullptr;

	sampler = 0;
	shadow = new ShadowMap();
	shadowShader = 0;

	samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	shadowMapSize = 1024;

	shadowViewport = {};
	shadowViewport.Height = shadowMapSize;
	shadowViewport.Width = shadowMapSize;
	shadowViewport.MinDepth = 0.f;
	shadowViewport.MaxDepth = 1.f;
	shadowViewport.TopLeftX = 0;
	shadowViewport.TopLeftY = 0;

	roughnessMap = 0;
	metalnessMap = 0;
	psPBR = 0;
	cubeMap = new CubeMap();

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

	//if (portalTexture1)
	//	delete portalTexture1;

	//if (portalTexture2)
	//	delete portalTexture2;


	if (grassMaterial)
		delete grassMaterial;

	if (wallMaterial)
		delete wallMaterial;

	if (rockMaterial)
		delete rockMaterial;

	//if (portalMaterial1)
	//	delete portalMaterial1;

	//if (portalMaterial2)
	//	delete portalMaterial2;

	//if (portalMaterial3)
	//	delete portalMaterial3;

	//if (portalMaterial4)
	//	delete portalMaterial4;
	
	if (portalMaterial)
		delete portalMaterial;

	if (zPrepassDepthStencilState)
		zPrepassDepthStencilState->Release();

	if (texture)
		texture->Release();

	if (normalMap)
		normalMap->Release();

	if (shadow) delete shadow;

	if (shadowShader) delete shadowShader;

	if (metalnessMap) metalnessMap->Release();

	if (roughnessMap) roughnessMap->Release();

	if (psPBR) delete psPBR;

	if (cubeMap) delete cubeMap;

	for (auto& depthStencilState : portalDepthStencilStates)
	{
		if (depthStencilState)
			depthStencilState->Release();
	}


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
	CreateDepthStencilStates();
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

	shadowShader = new SimpleVertexShader(device, context);
	bool isOK = shadowShader->LoadShaderFile((wpath + std::wstring(L"/shadowVS.cso")).c_str());
	if (!isOK) printf("shadow shader not loaded.\n");

	psPBR = new SimplePixelShader(device, context);
	psPBR->LoadShaderFile((wpath + std::wstring(L"/PixelShaderPBR.cso")).c_str());
	if (!isOK) printf("pbr ps not loaded.\n");

	cubeMap->setPS(device, context, (wpath + std::wstring(L"/cubePS.cso")).c_str());
	cubeMap->setVS(device, context, (wpath + std::wstring(L"/cubeVS.cso")).c_str());


	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	device->CreateDepthStencilState(&depthStencilDesc, &zPrepassDepthStencilState);
}

void Game::CreateDepthStencilStates()
{
	D3D11_DEPTH_STENCIL_DESC portalDSDesc = {};
	portalDSDesc.DepthEnable = true;
	portalDSDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	portalDSDesc.DepthFunc = D3D11_COMPARISON_LESS;

	portalDSDesc.StencilEnable = true;
	portalDSDesc.StencilReadMask = 0xFF;
	portalDSDesc.StencilWriteMask = 0xFF;

	portalDSDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_INCR;
	portalDSDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	portalDSDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	portalDSDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;

	portalDSDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	portalDSDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	portalDSDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	portalDSDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

	device->CreateDepthStencilState(&portalDSDesc, &portalDepthStencilStates.at(IncrementStencil));

	portalDSDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_DECR;
	device->CreateDepthStencilState(&portalDSDesc, &portalDepthStencilStates.at(DecrementStencil));

	portalDSDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	portalDSDesc.StencilWriteMask = 0;
	portalDSDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	device->CreateDepthStencilState(&portalDSDesc, &portalDepthStencilStates.at(DrawAtMaxRecursion));

	portalDSDesc.FrontFace.StencilFunc = D3D11_COMPARISON_LESS_EQUAL;
	device->CreateDepthStencilState(&portalDSDesc, &portalDepthStencilStates.at(DrawAtCurrentRecursion));

	portalDSDesc.StencilEnable = false;
	//portalDSDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	device->CreateDepthStencilState(&portalDSDesc, &portalDepthStencilStates.at(DrawToDepth));
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

	grassMaterial = new Material(vertexShader, pixelShader, grassTexture->GetResourceView(), grassTexture->GetSamplerState());
	wallMaterial = new Material(vertexShader, pixelShader, wallTexture->GetResourceView(), wallTexture->GetSamplerState());
	rockMaterial = new Material(vertexShader, pixelShader, rockTexture->GetResourceView(), rockTexture->GetSamplerState());
	/*portalMaterial1 = new Material(vsPortal, psPortal, portalTexture1->GetResourceView(), portalTexture1->GetSamplerState());
	portalMaterial2 = new Material(vsPortal, psPortal, portalTexture2->GetResourceView(), portalTexture2->GetSamplerState());
	portalMaterial3 = new Material(vsPortal, psPortal, portalTexture3->GetResourceView(), portalTexture3->GetSamplerState());
	portalMaterial4 = new Material(vsPortal, psPortal, portalTexture4->GetResourceView(), portalTexture4->GetSamplerState());*/

	//test for pbr
	device->CreateSamplerState(&samplerDesc, &sampler);

	HRESULT isok = CreateWICTextureFromFile(device, context, L"../Assets/Textures/PBR/floor_albedo.png", 0, &texture);
	if (FAILED(isok)) printf("load albedo texture error\n");

	isok = CreateWICTextureFromFile(device, context, L"../Assets/Textures/PBR/floor_normals.png", 0, &normalMap);
	if (FAILED(isok)) printf("load normal map error\n");

	isok = CreateWICTextureFromFile(device, context, L"../Assets/Textures/PBR/floor_roughness.png", 0, &roughnessMap);
	if (FAILED(isok)) printf("load roughnessmap error\n");

	isok = CreateWICTextureFromFile(device, context, L"../Assets/Textures/PBR/floor_metal.png", 0, &metalnessMap);
	if (FAILED(isok)) printf("load metalnessmap error\n");

	Material* pbrMaterial = new Material(vertexShader, psPBR, texture ,sampler);
	pbrMaterial->setTexture(texture);
	pbrMaterial->setMetalnessMap(metalnessMap);
	pbrMaterial->setRoughnessMap(roughnessMap);
	//pbrMaterial.setMetalnessMap(metalnessMap);
	pbrMaterial->setNormalMap(normalMap);
	//pbrMaterial.setRoughnessMap(roughnessMap);

	shadow->setUp(device);
	shadow->setShader(shadowShader);
	RenderingEngine* renderingEngine = RenderingEngine::GetSingleton();
	renderingEngine->setShadow(shadow);

	cubeMap->setUp(device);
	cubeMap->setMesh(cubeMesh);
	cubeMap->setSampler(sampler);
	cubeMap->setSRV(device, context, L"../Assets/Textures/CubeMaps/Skybox1.dds");
	//

	portalMaterial = new Material(vsPortal, psPortal, nullptr, nullptr);
	
	GameObject* directionalLightObject = new GameObject();
	directionalLightObject->SetRotation(45, 0, 0);
	directionalLight = directionalLightObject->AddComponent<DirectionalLight>();


	cameraObject = new GameObject();
	cameraObject->SetRotation(camRotX, camRotY, 0);
	//cameraObject->AddComponent<Renderer>(rockMaterial, sphereMesh);
	cameraObject->AddComponent<SphereCollider>(0.5f);
	cameraObject->AddComponent<CameraController>();
	cameraObject->SetPosition(0, 0, 0);
	cameraObject->AddTag("Player");

	GameObject* go = new GameObject();
	go->SetParent(cameraObject);
	go->SetLocalRotation(45, 0, 0);
	go->Translate(0, 0, 0);
	camera = go->AddComponent<Camera>();

	portalCamera1 = (new GameObject())->AddComponent<Camera>();
	portalCamera1->SetRatio((float)width / height);

	portalCamera2 = (new GameObject())->AddComponent<Camera>();
	portalCamera2->SetRatio((float)width / height);

	portalCamera3 = (new GameObject())->AddComponent<Camera>();
	portalCamera3->SetRatio((float)width / height);

	portalCamera4 = (new GameObject())->AddComponent<Camera>();
	portalCamera4->SetRatio((float)width / height);

	portal1 = __CreatePortal(portalMaterial, -15, 0, 0, 0, 0, 0);
	portal2 = __CreatePortal(portalMaterial, 5, 0, 0, 0, 180, 0);
	portal3 = __CreatePortal(portalMaterial, -15, 0, 10, 0, 180, 0);
	portal4 = __CreatePortal(portalMaterial, 5, 0, 20, 0, 0, 0);

	portal1->SetExit(portal2);
	portal1->SetMainCamera(camera);
	portal1->SetViewCamera(portalCamera1);

	portal2->SetExit(portal1);
	portal2->SetMainCamera(camera);
	portal2->SetViewCamera(portalCamera2);

	portal3->SetExit(portal4);
	portal3->SetMainCamera(camera);
	portal3->SetViewCamera(portalCamera3);

	portal4->SetExit(portal3);
	portal4->SetMainCamera(camera);
	portal4->SetViewCamera(portalCamera4);

	portalCamera1->GetGameObject()->SetParent(portal2->GetGameObject());
	portalCamera1->GetGameObject()->SetLocalPosition(0, 0, 0);
	portalCamera2->GetGameObject()->SetParent(portal1->GetGameObject());
	portalCamera2->GetGameObject()->SetLocalPosition(0, 0, 0);
	portalCamera3->GetGameObject()->SetParent(portal4->GetGameObject());
	portalCamera3->GetGameObject()->SetLocalPosition(0, 0, 0);
	portalCamera4->GetGameObject()->SetParent(portal3->GetGameObject());
	portalCamera4->GetGameObject()->SetLocalPosition(0, 0, 0);

	floor = new GameObject();
	floor->SetPosition(0, -2, 0);
	floor->SetLocalScale(100, 100, 1);
	floor->SetRotation(-90, 0, 0);
	floor->AddComponent<Renderer>(grassMaterial, floorMesh);
	
	tunnel1 = new GameObject();
	tunnel1->SetPosition(-15, -2, 5);
	tunnel1->SetLocalScale(2, 2, 5);
	tunnel1->AddComponent<Renderer>(rockMaterial, tunnelMesh);

	GameObject* wallCollider1L = new GameObject();
	wallCollider1L->SetParent(tunnel1);
	wallCollider1L->SetLocalPosition(0, 0, 0);
	wallCollider1L->AddComponent<BoxCollider>(XMVECTOR{ 0.4, 8, 10 }, XMVECTOR{ -1.5,0,0 });
	wallCollider1L->AddTag("Wall");

	GameObject* wallCollider1R = new GameObject();
	wallCollider1R->SetParent(tunnel1);
	wallCollider1R->SetLocalPosition(0, 0, 0);
	wallCollider1R->AddComponent<BoxCollider>(XMVECTOR{ 0.4, 8, 10 }, XMVECTOR{ 1.5,0,0 });
	wallCollider1R->AddTag("Wall");

	tunnel2 = new GameObject();
	tunnel2->SetPosition(5, -2, 10);
	tunnel2->SetLocalScale(2, 2, 10);
	tunnel2->AddComponent<Renderer>(rockMaterial, tunnelMesh);

	cube = new GameObject();
	cube->SetPosition(-15, 0, -1);
	cube->AddComponent<Renderer>(grassMaterial, cubeMesh);


	GameObject* wallCollider2L = new GameObject();
	wallCollider2L->SetParent(tunnel2);
	wallCollider2L->SetLocalPosition(0, 0, 0);
	wallCollider2L->AddComponent<BoxCollider>(XMVECTOR{ 0.4, 8, 20 }, XMVECTOR{ 1.5,0,0 });
	wallCollider2L->AddTag("Wall");


	GameObject* wallCollider2R = new GameObject();
	wallCollider2R->SetParent(tunnel2);
	wallCollider2R->SetLocalPosition(0, 0, 0);
	wallCollider2R->AddComponent<BoxCollider>(XMVECTOR{ 0.4, 8, 20 }, XMVECTOR{ -1.5,0,0 });
	wallCollider2R->AddTag("Wall");



	GameObject* button = new GameObject();
	button->SetPosition(-4, -2, 2);
	button->SetLocalScale(2, 2, 2);
	button->AddComponent<BoxCollider>(XMVECTOR{ 2, 2, 2 }, XMVECTOR{ 0,0,0 });
	button->AddTag("Wall");


	GameObject* triggerBox = new GameObject();
	triggerBox->SetPosition(-4, 0, 2);
	triggerBox->SetLocalScale(2, 2, 2);
	triggerBox->AddComponent<BoxCollider>(XMVECTOR{ 3, 3, 3}, XMVECTOR{ 0,0,0 });
	triggerBox->AddComponent<Renderer>(pbrMaterial, cubeMesh);
	triggerBox->AddTag("TriggerBox");

	GameObject* panel = new GameObject();
	panel->SetPosition(-15, 0, 0);
	panel->SetLocalScale(4, 5, 0.5);
	//panel->AddComponent<BoxCollider>(XMVECTOR{ 1, 1, 1 }, XMVECTOR{ 0,0,0 });
	panel->AddComponent<Renderer>(rockMaterial, cubeMesh);
	panel->AddTag("Wall");
	panel->AddTag("TriggerPanel");
	panel->AddComponent<TriggerTarget>(XMVECTOR{ -15, 5, 0 });

	triggerBox->AddComponent<Trigger>(panel->GetComponent<TriggerTarget>(), XMVECTOR{ -4, -2.5f, 2 });
}

__forceinline Portal* Game::__CreatePortal(Material* material, f32 x, f32 y, f32 z, f32 rx, f32 ry, f32 rz)
{
	GameObject* portal = new GameObject();

	GameObject* portalArea1 = new GameObject();
	portalArea1->SetParent(portal);
	portalArea1->SetLocalRotation(0, 0, 0);
	portalArea1->SetLocalScale(1.2f, 2.0f, 1);
	auto portalRenderer = portalArea1->AddComponent<Renderer>(material, quadMesh);
	portalArea1->AddComponent<BoxCollider>(XMVECTOR{ 2, 2, 0.1f }, XMVECTOR{ 0,0,0 });
	portalArea1->AddTag("Portal");

	RenderingEngine* renderingEngine = RenderingEngine::GetSingleton();
	renderingEngine->AddPortal(portalRenderer);

	/*GameObject* pillar1L = new GameObject();
	pillar1L->SetParent(portal);
	pillar1L->SetLocalPosition(-2.5f, 0, 0);
	pillar1L->SetLocalScale(0.2f, 5, 0.2f);
	pillar1L->AddComponent<Renderer>(wallMaterial, cubeMesh);

	GameObject* pillar1R = new GameObject();
	pillar1R->SetParent(portal);
	pillar1R->SetLocalPosition(2.5f, 0, 0);
	pillar1R->SetLocalScale(0.2f, 5, 0.2f);
	pillar1R->AddComponent<Renderer>(wallMaterial, cubeMesh);*/

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
	Scene::GetCurrentScene()->LateUpdate(deltaTime, totalTime);

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

	const float color[4] = { 0.69f, 0.88f, 0.9f, 0.0f };
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	shadowViewport.Height = shadowMapSize;
	shadowViewport.Width = shadowMapSize;
	context->RSSetViewports(1, &shadowViewport);

	renderingEngine->RenderShadowMap(context);

	context->OMSetRenderTargets(1, &backBufferRTV, depthStencilView);

	shadowViewport.Width = (float)width;
	shadowViewport.Height = (float)height;

	context->RSSetViewports(1, &shadowViewport);
	context->RSSetState(0);

	//renderingEngine->PerformZPrepass(vsZPrepass, context);

	//context->OMSetDepthStencilState(zPrepassDepthStencilState, 0);
	renderingEngine->DrawForward(context);
	//context->OMSetDepthStencilState(nullptr, 0);
	renderingEngine->DrawPortals(context, camera, portalDepthStencilStates, backBufferRTV, depthStencilView, 1);

	//renderingEngine->RenderCubeMap(context, cubeMap);

	ID3D11ShaderResourceView* noSRV[16] = {};
	context->PSSetShaderResources(0, 16, noSRV);

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

