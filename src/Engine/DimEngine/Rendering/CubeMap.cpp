#include "CubeMap.h"

DimEngine::Rendering::CubeMap::CubeMap() {
	//rasterizer desc
	cubeRD = {};
	cubeRD.CullMode = D3D11_CULL_FRONT;
	cubeRD.FillMode = D3D11_FILL_SOLID;
	cubeRD.DepthClipEnable = true;

	//depth stencil desc
	cubeDSD = {};
	cubeDSD.DepthEnable = true;
	cubeDSD.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	cubeDSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

	//shader for cube
	cubeVS = 0;
	cubePS = 0;

	//rasterizer state & depth stencil state & shader resource view
	cubeRS = 0;
	cubeDSS = 0;
	cubeSRV = 0;

	//sampler
	sampler = 0;

	//buffer
	vertexBuffer = 0;
	indicesNum = 0;
	indexBuffer = 0;
}

DimEngine::Rendering::CubeMap::~CubeMap() {
	if (cubeVS) delete cubeVS;
	if (cubePS) delete cubePS;

	if (cubeRS) cubeRS->Release();
	if (cubeDSS) cubeDSS->Release();
	if (cubeSRV) cubeSRV->Release();
}

bool DimEngine::Rendering::CubeMap::setUp(ID3D11Device * device) {
	bool isOk = false;

	isOk = device->CreateRasterizerState(&cubeRD, &cubeRS);
	isOk = device->CreateDepthStencilState(&cubeDSD, &cubeDSS);

	return isOk;
}

void DimEngine::Rendering::CubeMap::setSampler(ID3D11SamplerState * _sampler) {
	sampler = _sampler;
}

void DimEngine::Rendering::CubeMap::setMesh(Mesh * _cubeMesh) {
	vertexBuffer = _cubeMesh->GetVertexBuffer();
	indexBuffer = _cubeMesh->GetIndexBuffer();
	indicesNum = _cubeMesh->GetIndexCount();
}

void DimEngine::Rendering::CubeMap::setVS(ID3D11Device * device, ID3D11DeviceContext * context, LPCWSTR path) {
	cubeVS = new SimpleVertexShader(device, context);
	cubeVS->LoadShaderFile(path);
}

void DimEngine::Rendering::CubeMap::setPS(ID3D11Device * device, ID3D11DeviceContext * context, LPCWSTR path) {
	cubePS = new SimplePixelShader(device, context);
	cubePS->LoadShaderFile(path);
}

void DimEngine::Rendering::CubeMap::setSRV(ID3D11Device * device, ID3D11DeviceContext * context, const wchar_t* path) {
	CreateDDSTextureFromFile(device, context, path, 0, &cubeSRV);
	bool i = true;
}

ID3D11SamplerState * DimEngine::Rendering::CubeMap::getSampler() {
	return sampler;
}

SimpleVertexShader * DimEngine::Rendering::CubeMap::getVS() {
	return cubeVS;
}

SimplePixelShader * DimEngine::Rendering::CubeMap::getPS() {
	return cubePS;
}

ID3D11Buffer * DimEngine::Rendering::CubeMap::getVB() {
	return vertexBuffer;
}

ID3D11Buffer * DimEngine::Rendering::CubeMap::getIB() {
	return indexBuffer;
}

int DimEngine::Rendering::CubeMap::getIndexCount() {
	return indicesNum;
}

ID3D11RasterizerState * DimEngine::Rendering::CubeMap::getRS() {
	return cubeRS;
}

ID3D11DepthStencilState * DimEngine::Rendering::CubeMap::getDSS() {
	return cubeDSS;
}

ID3D11ShaderResourceView * DimEngine::Rendering::CubeMap::getSRV() {
	return cubeSRV;
}
