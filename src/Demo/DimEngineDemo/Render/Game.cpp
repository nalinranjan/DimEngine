#include <string>

#include "Game.h"

#include "Core/Global.h"

#include "Rendering/RenderingEngine.h"
#include "Rendering/Mesh.h"

using namespace DirectX;
using namespace DimEngine;
using namespace DimEngine::Rendering;

Game::Game(HINSTANCE hInstance, char* name) : DXCore(hInstance, name, 1280, 720, true)
{
	vertexShader = nullptr;
	pixelShader = nullptr;

	//entityVector.resize(7);
	//for (int countOfVector = 0; countOfVector < entityVector.size(); countOfVector++)
	//	entityVector[countOfVector] = NULL;

	//camera = new Camera();
	//physics = new CollisionManager(200);
	//simpleMaterial = NULL;

	//directionalLight.AmbientColor = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	//directionalLight.DiffuseColor = XMFLOAT4(0, 0, 0.5, 1);
	//directionalLight.Direction = XMFLOAT3(1, -1, 0);

	//pointLight.AmbientColor = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	//pointLight.DiffuseColor = XMFLOAT4(0.0f, 0.0f, 0.7f, 1.0f);
	//pointLight.Position = XMFLOAT3(0, 2.0f, -1.0f);

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
	LoadShaders();
	CreateMatrces();
	CreateBasicGeometry();
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
	psPortal->LoadShaderFile((wpath + std::wstring(L"/ps_portal")).c_str());

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
	char* filename = (char*)"../Assets/Models/sphere.obj";
	char* cubefile = (char*)"../Assets/Models/cube.obj";
	sphereMesh = new Mesh(device, filename);
	cubeMesh = new Mesh(device, cubefile);

	// D3D resources
	ID3D11Texture2D* texture;

	// Create texture
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Height = 512;
	textureDesc.Width = 512;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	device->CreateTexture2D(&textureDesc, nullptr, &texture);

	// Create Render Target View to bind the texture as a render target
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	device->CreateRenderTargetView(texture, &rtvDesc, &portalRTV);

	// Create Shader Resource View to bind the texture as a shader resource
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(texture, &srvDesc, &portalRSV);

	// Release reference to the texture buffer as it's no longer required
	texture->Release();

	//ID3D11SamplerState* basicSampler;
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &portalSampler);

	// Create Depth Stencil View for Render Targets
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = 512;
	depthStencilDesc.Height = 512;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	// Create the depth buffer and its view, then 
	// release our reference to the texture
	ID3D11Texture2D* depthBufferTexture;
	device->CreateTexture2D(&depthStencilDesc, 0, &depthBufferTexture);
	device->CreateDepthStencilView(depthBufferTexture, 0, &portalDSV);
	depthBufferTexture->Release();

	simpleMaterial = new Material(vertexShader, pixelShader, nullptr, nullptr);
	portalMaterial = new Material(vertexShader, psPortal, portalRSV, portalSampler);
	
	camera = new GameObject();
	camera->SetLocalRotation(0, -180, 0);
	camera->AddComponent<Camera>();

	portalCamera = (new GameObject())->AddComponent<Camera>();
	portalCamera->SetRenderTarget(portalRTV);

	GameObject* directionalLightObject = new GameObject();
	directionalLightObject->SetRotation(90, 0, 0);
	directionalLight = directionalLightObject->AddComponent<DirectionalLight>();
	
	floor = new GameObject();
	floor->SetPosition(0, -2, 0);
	floor->SetLocalScale(100, 0.1f, 100);
	floor->AddComponent<Renderer>(simpleMaterial, cubeMesh);

	portal = new GameObject();
	portal->SetLocalPosition(0, 0, 10);
	portal->SetLocalScale(2, 5, 0.1f);
	portal->AddComponent<Renderer>(portalMaterial, cubeMesh);

	cube = new GameObject();
	cube->SetLocalPosition(0, 0, 15);
	cube->AddComponent<Renderer>(simpleMaterial, cubeMesh);

	sphere = new GameObject();
	sphere->SetLocalPosition(0, 0, -5);
	sphere->AddComponent<Renderer>(simpleMaterial, sphereMesh);
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

	if (GetAsyncKeyState('W') & 0x8000)
		camera->Translate(0.0f, 0.0f, deltaTime);

	if (GetAsyncKeyState('A') & 0x8000)
		camera->Translate(-deltaTime, 0.0f, 0.0f);

	if (GetAsyncKeyState('S') & 0x8000)
		camera->Translate(0.0f, 0.0f, -deltaTime);

	if (GetAsyncKeyState('D') & 0x8000)
		camera->Translate(deltaTime, 0.0f, 0.0f);

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		camera->Translate(0.0f, deltaTime, 0.0f);

	if (GetAsyncKeyState('X') & 0x8000)
		camera->Translate(0.0f, -deltaTime, 0.0f);

	XMVECTOR v = camera->GetPosition();
	printf("( %f, %f, %f, %f )\n", v.m128_f32[0], v.m128_f32[1], v.m128_f32[2], v.m128_f32[3]);
}

void Game::Draw(float deltaTime, float totalTime)
{
	RenderingEngine* renderingEngine = RenderingEngine::GetSingleton();
	renderingEngine->UpdateRenderables();
	renderingEngine->UpdateViewers();
	renderingEngine->UpdateLightSources();
	renderingEngine->SortRenderables();


	//context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//context->OMSetDepthStencilState(nullptr, 0);

	//portalCamera->RenderToRenderTarget(context, depthStencilView);

	//D3D11_VIEWPORT viewport = {};
	//viewport.TopLeftX = 0;
	//viewport.TopLeftY = 0;
	//viewport.Width = (float)width;
	//viewport.Height = (float)height;
	//viewport.MinDepth = 0.0f;
	//viewport.MaxDepth = 1.0f;
	//context->RSSetViewports(1, &viewport);


	const float color[4] = { 0.4f, 1, 1, 0 };

	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &backBufferRTV, depthStencilView);

	renderingEngine->PerformZPrepass(vsZPrepass, context);
	
	context->OMSetDepthStencilState(zPrepassDepthStencilState, 0);
	renderingEngine->DrawForward(context);

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
		camera->Rotate((y - prevMousePos.y) / 31.41592653579f, (x - prevMousePos.x) / 31.41592653579f, 0.0f);

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

