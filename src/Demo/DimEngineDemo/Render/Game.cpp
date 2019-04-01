﻿#include <string>

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
	//simpleMaterial = new Material(vertexShader, pixelShader, 0, 0);
	char* filename = (char*)"../Assets/Models/sphere.obj";
	char* cubefile = (char*)"../Assets/Models/cube.obj";
	mesh = new Mesh(device, filename);
	mesh1 = new Mesh(device, cubefile);
	//Entity* temp = new Entity(mesh, simpleMaterial);
	//Entity* temp1 = new Entity(mesh1, simpleMaterial);
	//Entity* temp2 = new Entity(mesh1, simpleMaterial);
	//Entity* temp3 = new Entity(mesh, simpleMaterial);
	//Entity* temp4 = new Entity(mesh, simpleMaterial);
	//Entity* temp5 = new Entity(mesh, simpleMaterial);
	//Entity* terrain = new Entity(mesh1, simpleMaterial);
	//entityVector[0] = temp;
	//entityVector[1] = temp1;
	//entityVector[2] = temp2;
	//entityVector[3] = temp3;
	//entityVector[4] = temp4;
	//entityVector[5] = temp5;
	//entityVector[6] = terrain;
	//temp->SetTranslation(-2, 0, 0);
	//temp1->SetTranslation(2.5, 1.5, 0);
	//temp2->SetTranslation(-2.5, 1, 0);
	//temp3->SetTranslation(0.0, 1, 0);
	//temp4->SetTranslation(-1, -1, 0);
	//temp5->SetTranslation(2, 0, 0);
	////temp1->SetRotation(0.5f, 0.0f, 0.0f);
	//terrain->SetTranslation(0, -10, 0);
	//terrain->SetScale(100, 1, 100);
	//Collider* collider = physics->addSphereCollider(temp, 0.5f, 0.5f, true, false);
	//Collider* collider1 = physics->addBoxCollider(temp1, XMFLOAT3{ 1,1,1 }, 0.7f, true, false);
	//Collider* collider2 = physics->addBoxCollider(temp2, XMFLOAT3{ 1,1,1 }, 0.4f, true, false);
	//Collider* collider3 = physics->addSphereCollider(temp3, 0.5f, 0.7f, true, false);
	//Collider* collider4 = physics->addSphereCollider(temp4, 0.5f, 0.5f, true, false);
	//Collider* collider5 = physics->addSphereCollider(temp5, 0.5f, 0.7f, true, false);
	//Collider* collider6 = physics->addBoxCollider(terrain,XMFLOAT3{100,1,100}, 1.0f, false, true);
	//collider->ApplyForce({ 0.5f,0,0.1f });
	//collider1->ApplyForce({ -1.5f,0,0.0f });
	////collider1->ApplyAngularForce({ -0.5f,0.0f,0.0f });
	//collider2->ApplyForce({ 1.5f,0,0.0f });
	//collider3->ApplyForce({ -0.9f,0,0.1f });
	//collider4->ApplyForce({ 1.0f,0,-0.1f });
	//collider5->ApplyForce({ -1.4f,0,0.1f });

	simpleMaterial = new Material(vertexShader, pixelShader, nullptr, nullptr);
	
	camera = new GameObject();
	camera->AddComponent<Camera>();

	GameObject* directionalLightObject = new GameObject();
	directionalLightObject->SetRotation(-90, 0, 0);
	directionalLight = directionalLightObject->AddComponent<DirectionalLight>();
	
	go1 = new GameObject();
	go1->SetPosition(0, 0, 10);
	go1->SetLocalRotation(45, 0, 0);
	go1->SetLocalScale(1, 2, 1);
	go1->AddComponent<Renderer>(simpleMaterial, mesh1);

	go2 = new GameObject();
	go2->SetParent(go1);
	go2->SetLocalPosition(0, 4, 0);
	go2->AddComponent<Renderer>(simpleMaterial, mesh1);

	GameObject* go3 = new GameObject();
	go3->SetParent(go2);
	go3->SetLocalPosition(0, 2, 0);
	go3->AddComponent<Renderer>(simpleMaterial, mesh);

	GameObject* go4 = new GameObject();
}

void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	Global::SetScreenRatio((float)width / height);

	//XMMATRIX projection = camera->UpdateProjection((float)width / height);
	//XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(projection));
}

void Game::Update(float deltaTime, float totalTime)
{
	//physics->CollisionsDetection(0, physics->NumCoolidersHandled, deltaTime, totalTime);
	
	if (GetAsyncKeyState(VK_ESCAPE)) Quit();

	//XMMATRIX view = camera->Update();
	//
	//XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(view));

	if (GetAsyncKeyState('W') & 0x8000)
		camera->Translate(0.0f, 0.0f, deltaTime, SELF);

	if (GetAsyncKeyState('A') & 0x8000)
		camera->Translate(-deltaTime, 0.0f, 0.0f, SELF);

	if (GetAsyncKeyState('S') & 0x8000)
		camera->Translate(0.0f, 0.0f, -deltaTime, SELF);

	if (GetAsyncKeyState('D') & 0x8000)
		camera->Translate(deltaTime, 0.0f, 0.0f, SELF);

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		camera->Translate(0.0f, deltaTime, 0.0f, SELF);

	if (GetAsyncKeyState('X') & 0x8000)
		camera->Translate(0.0f, -deltaTime, 0.0f, SELF);

	go1->Rotate(0, 0, 20 * deltaTime);
	go2->Rotate(0, 0, -50 * deltaTime);
}

void Game::Draw(float deltaTime, float totalTime)
{
	//backgroud color
	const float color[4] = { 0, 0, 0, 0 };

	//-set backgroud color
	//-clear depth buffer
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//
	////-------------------------------------

	//for (int countOfEntity = 0; countOfEntity < entityVector.size(); countOfEntity++) {
	//	entityVector[countOfEntity]->SetWorldMatrix();
	//	entityVector[countOfEntity]->PrepareMatrix(viewMatrix, projectionMatrix);
	//	entityVector[countOfEntity]->SetPointLight(pointLight, "pointLight");
	//	entityVector[countOfEntity]->SetLight(directionalLight, "light");
	//	entityVector[countOfEntity]->CopyAllBufferData();
	//	entityVector[countOfEntity]->SetShader();
	//	//set vertex buffer and index buffer inside entity class
	//	entityVector[countOfEntity]->Draw(context);
	//}

	////-------------------------------------

	RenderingEngine* renderingEngine = RenderingEngine::GetSingleton();
	renderingEngine->UpdateRenderables();
	renderingEngine->UpdateViewers();
	renderingEngine->UpdateLightSources();
	renderingEngine->SortRenderables();

	context->OMSetDepthStencilState(nullptr, 0);
	renderingEngine->PerformZPrepass(vsZPrepass, context);
	
	context->OMSetDepthStencilState(zPrepassDepthStencilState, 0);
	renderingEngine->DrawForward(context);

	//End of rendering one frame
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

