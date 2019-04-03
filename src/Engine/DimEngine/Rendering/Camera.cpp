#include "Camera.h"
#include "RenderingEngine.h"

DimEngine::Camera::Camera()
{
	renderTarget = nullptr;

	fov = 0.25f * 3.1415926535f;
	nearZ = 0.1f;
	farZ = 100;

	viewer = null_index;

	RenderingEngine::GetSingleton()->AddCamera(this);
}

DimEngine::Camera::~Camera()
{
	if (viewer != null_index)
		RenderingEngine::GetSingleton()->DestroyViewer(viewer);

	RenderingEngine::GetSingleton()->RemoveCamera(this);
}

void DimEngine::Camera::SetRenderTarget(ID3D11RenderTargetView * renderTarget)
{
	this->renderTarget = renderTarget;
}

void DimEngine::Camera::SetFov(f32 value)
{
	fov = value;
}

void DimEngine::Camera::SetNearZ(f32 value)
{
	nearZ = value;
}

void DimEngine::Camera::SetFarZ(f32 value)
{
	farZ = value;
}

void DimEngine::Camera::RenderToRenderTarget(ID3D11DeviceContext* context, ID3D11DepthStencilView* depthStencilView)
{
	if (renderTarget)
	{
		D3D11_VIEWPORT rtViewport = {};
		rtViewport.TopLeftX = 0;
		rtViewport.TopLeftY = 0;
		rtViewport.Width = 512;
		rtViewport.Height = 512;
		rtViewport.MinDepth = 0.0f;
		rtViewport.MaxDepth = 1.0f;
		context->RSSetViewports(1, &rtViewport);

		const float color[4] = { 0.69f, 0.88f, 0.9f, 0.0f };



		// Render to textures

		context->ClearRenderTargetView(renderTarget, color);
		context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		context->OMSetRenderTargets(1, &renderTarget, depthStencilView);

		RenderingEngine::GetSingleton()->DrawForward(context, this);

		// Set the viewport back to its original state
		//rtViewport.Width = (float)width;
		//rtViewport.Height = (float)height;
		//context->RSSetViewports(1, &rtViewport);
	}
}

//void Camera::SetRotationX(float _rotationX) {
//	rotationX = _rotationX;
//}
//
//void Camera::SetRotationY(float _rotationY) {
//	rotationY = _rotationY;
//}
//
//XMMATRIX Camera::Update() {
//	//if (GetAsyncKeyState(VK_SPACE) & 0x8000) { position.y += -0.0001f; }
//	//if (GetAsyncKeyState('X') & 0x8000) { position.y -= -0.0001f; }
//
//	//rotation quaternion
//	XMVECTOR rotation = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&XMFLOAT3(rotationX, rotationY, 0)));
//	XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(rotation);
//
//	//applying rotation quaternion to direction
//	XMVECTOR directionVector = XMVector3Transform(XMLoadFloat3(&direction), rotationMatrix);
//	XMStoreFloat3(&direction, directionVector);
//
//	//speed needs to be set
//
//	//move forward
//	//if (GetAsyncKeyState('W') & 0x8000) {
//	//	position.x += direction.x * 0.001f;
//	//	position.y += direction.y * 0.001f;
//	//	position.z += direction.z * 0.001f;
//	//}
//
//	////move backward
//	//if (GetAsyncKeyState('S') & 0x8000) {
//	//	position.x -= direction.x * 0.001f;
//	//	position.y -= direction.y * 0.001f;
//	//	position.z -= direction.z * 0.001f;
//	//}
//
//	XMFLOAT3 upDirection = XMFLOAT3(0, 1.0f, 0);
//
//	//left direction
//	XMVECTOR leftVector;
//	XMFLOAT3 leftDirection;
//	leftVector = XMVector3Cross(XMLoadFloat3(&upDirection), XMLoadFloat3(&direction));
//
//	XMStoreFloat3(&leftDirection, leftVector);
//
//	//if (GetAsyncKeyState('A') & 0x8000) {
//	//	position.x += leftDirection.x * 0.001f;
//	//	position.y += leftDirection.y * 0.001f;
//	//	position.z += leftDirection.z * 0.001f;
//	//}
//
//	//if (GetAsyncKeyState('D') & 0x8000) {
//	//	position.x -= leftDirection.x * 0.001f;
//	//	position.y -= leftDirection.y * 0.001f;
//	//	position.z -= leftDirection.z * 0.001f;
//	//}
//
//	//generate view matrix
//	XMMATRIX viewMatrix = XMMatrixLookToLH(XMLoadFloat3(&position), directionVector, XMLoadFloat3(&upDirection));
//
//	//reset rotation
//	rotationX = 0.0f;
//	rotationY = 0.0f;
//
//	return viewMatrix;
//}
//
//XMMATRIX Camera::UpdateProjection(float screenRatio)
//{
//	XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(
//		0.25f * 3.1415926535f,		// Field of View Angle
//		(float)screenRatio,			// Aspect ratio
//		0.1f,						// Near clip plane distance
//		100.0f);					// Far clip plane distance
//
//	return projectionMatrix;
//}