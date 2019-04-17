#include "Texture.h"

using namespace DirectX;

DimEngine::Rendering::Texture::Texture()
{
	resourceView = nullptr;
	samplerState = nullptr;
}

DimEngine::Rendering::Texture::Texture(wchar_t* _filepath, D3D11_TEXTURE_ADDRESS_MODE _addressMode, D3D11_FILTER _filterMode, f32 _maxLOD, ID3D11Device* _device, ID3D11DeviceContext* _context) {

	ID3D11Resource* texture = nullptr;
	CreateWICTextureFromFile(_device, _context, _filepath, &texture, &resourceView);

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = _addressMode;
	samplerDesc.AddressV = _addressMode;
	samplerDesc.AddressW = _addressMode;
	samplerDesc.Filter = _filterMode;
	samplerDesc.MaxLOD = _maxLOD;
	_device->CreateSamplerState(&samplerDesc, &samplerState);

	texture->Release();
}

DimEngine::Rendering::Texture::~Texture() {
	if (resourceView) resourceView->Release();
	if (samplerState) samplerState->Release();
}

DimEngine::Rendering::RenderTexture::RenderTexture(ID3D11Device* device, u32 size) : RenderTexture(device, size, size)
{
}

DimEngine::Rendering::RenderTexture::RenderTexture(ID3D11Device* device, u32 width, u32 height)
{
	this->width = width;
	this->height = height;


	ID3D11Texture2D* texture;

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
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

	device->CreateRenderTargetView(texture, &rtvDesc, &renderTargetView);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(texture, &srvDesc, &resourceView);

	texture->Release();


	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &samplerState);


	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	ID3D11Texture2D* depthBufferTexture;
	device->CreateTexture2D(&depthStencilDesc, 0, &depthBufferTexture);
	device->CreateDepthStencilView(depthBufferTexture, 0, &depthStencilView);
	depthBufferTexture->Release();
}

DimEngine::Rendering::RenderTexture::~RenderTexture()
{
	renderTargetView->Release();
	depthStencilView->Release();
}

__inline ID3D11RenderTargetView * DimEngine::Rendering::RenderTexture::GetRenderTargetView()
{
	return renderTargetView;
}

__inline ID3D11DepthStencilView * DimEngine::Rendering::RenderTexture::GetDepthStencilView()
{
	return depthStencilView;
}
