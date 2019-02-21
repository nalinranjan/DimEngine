#include "Game.h"
#include "Vertex.h"
#include "InputManager.h"
#include "WICTextureLoader.h"

#define MOUSE_LEFT_DOWN (buttonState & 0x0001)
#define MOUSE_RIGHT_DOWN (buttonState & 0x0002)

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		// The application's handle
		"DirectX Game",	   	// Text for the window's title bar
		1280,			// Width of the window's client area
		720,			// Height of the window's client area
		true)			// Show extra stats (fps) in title bar?
{
	// Initialize fields
	vertexShader = 0;
	pixelShader = 0;

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif
	
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	delete camera;

	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;

	// Delete Input singleton last
	delete Input::Instance();

}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	scene.Init(device, context);

	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateMatrices();
	CreateBasicGeometry();

	directionalLight = {
		XMFLOAT4(0.08f, 0.12f, 0.15f, 1.0f),
		XMFLOAT4(1, 1, 1, 1),
		XMFLOAT3(1, -1, 0)
	};

	directionalLight2 = {
		XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		XMFLOAT4(1, 1, 0, 1),
		XMFLOAT3(0, -1, 0)
	};

	pointLight = {
		XMFLOAT4(0, 0, 0, 1),
		XMFLOAT4(0, 0, 0, 1),
		XMFLOAT3(2, 0, 2)
	};

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");
}



// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{
	// Initialize camera
	camera = new Camera(true);
	camera->GetTransform()->SetPosition(0, 0, -5);
	camera->SetProjection((float)width / height);
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Load meshes
	scene.CreateMesh("cone", "Models/cone.obj");
	scene.CreateMesh("cube", "Models/cube.obj");
	scene.CreateMesh("cylinder", "Models/cylinder.obj");
	scene.CreateMesh("helix", "Models/helix.obj");
	scene.CreateMesh("sphere", "Models/sphere.obj");
	scene.CreateMesh("torus", "Models/torus.obj");

	Texture* crateTexture = scene.CreateTexture("crate", L"Textures/crate.png", D3D11_TEXTURE_ADDRESS_WRAP, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_FLOAT32_MAX);
	Texture* stoneTexture = scene.CreateTexture("stone", L"Textures/grass.jpg", D3D11_TEXTURE_ADDRESS_WRAP, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_FLOAT32_MAX);

	// Create materials
	scene.CreateMaterial("default", vertexShader, pixelShader, crateTexture);
	scene.CreateMaterial("stone", vertexShader, pixelShader, stoneTexture);

	// Create GameObjects
	Material* defaultMaterial = scene.GetMaterial("default");
	Material* stoneMaterial = scene.GetMaterial("stone");

	Renderable* sphereObj = scene.CreateObject("sphere", scene.GetMesh("sphere"), stoneMaterial);
	sphereObj->GetTransform()->SetPosition(-4, 0, 0);

	Renderable* coneObj = scene.CreateObject("cone", scene.GetMesh("cone"), stoneMaterial);
	coneObj->GetTransform()->SetPosition(4, 0, 0);

	Renderable* cubeObj = scene.CreateObject("cube", scene.GetMesh("cube"), defaultMaterial);
	cubeObj->GetTransform()->SetPosition(4, 0, 4);

	Renderable* helixObj = scene.CreateObject("helix", scene.GetMesh("helix"), defaultMaterial);
	helixObj->GetTransform()->SetPosition(0, 0, 4);

}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	// Update our projection matrix since the window size changed
	camera->SetProjection((float)width / height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	// Move objects to test functionality
	float centerCircleScale = sinf(totalTime * 2.f) / 20.f + 0.5f;
	Renderable* sphere = scene.GetGameObject("sphere");
	sphere->GetTransform()->SetPosition(0, sinf(totalTime * 2.f), 0);
	sphere->GetTransform()->Rotate(0, 0, deltaTime);

	scene.GetGameObject("cone")->GetTransform()->Rotate(deltaTime, 0, 0);

	scene.GetGameObject("cube")->GetTransform()->Rotate(0, deltaTime, deltaTime / 3.f);

	scene.GetGameObject("helix")->GetTransform()->Rotate(0, deltaTime, 0);

	camera->Update(deltaTime);

	Input::Instance()->EndFrame();

}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float backgroundColor[4] = { 0.4f, 0.6f, 0.75f, 0.0f };
	//const float backgroundColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, backgroundColor);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	pixelShader->SetData(
		"directionalLight",
		&directionalLight,
		sizeof(DirectionalLight)
	);

	pixelShader->SetData(
		"directionalLight2",
		&directionalLight2,
		sizeof(DirectionalLight)
	);

	pixelShader->SetData(
		"pointLight",
		&pointLight,
		sizeof(PointLight)
	);

	std::map<std::string, Renderable*> gameobjects = scene.GetAllGameObjects();
	for each (const auto &element in gameobjects) {

		Renderable* gObj = element.second;

		// Prepare object to be rendered to the scene
		gObj->PrepareMaterial(camera->GetViewMatrix(), camera->GetProjectionMatrix());

		Mesh* mesh = gObj->GetMesh();

		// Set buffers in the input assembler
		//  - Do this ONCE PER OBJECT you're drawing, since each object might
		//    have different geometry.
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		ID3D11Buffer* meshVertexBuffer = mesh->GetVertexBuffer();
		ID3D11Buffer* meshIndexBuffer = mesh->GetIndexBuffer();
		context->IASetVertexBuffers(0, 1, &meshVertexBuffer, &stride, &offset);
		context->IASetIndexBuffer(meshIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		context->DrawIndexed(mesh->GetIndexCount(), 0, 0);

	}

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
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
	if (MOUSE_LEFT_DOWN) Input::Instance()->SetMouseState(0, true);
	if (MOUSE_RIGHT_DOWN) Input::Instance()->SetMouseState(2, true);

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
	if (!MOUSE_LEFT_DOWN) Input::Instance()->SetMouseState(0, false);
	if (!MOUSE_RIGHT_DOWN) Input::Instance()->SetMouseState(2, false);

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
	// Save the previous mouse position, so we have it for the future
	Input::Instance()->UpdateMousePosition(x, y);
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