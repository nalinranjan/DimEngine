#include "Texture.h"

using namespace DirectX;

DimEngine::Rendering::Texture::Texture(wchar_t* _filepath, D3D11_TEXTURE_ADDRESS_MODE _addressMode, D3D11_FILTER _filterMode, float _maxLOD, ID3D11Device* _device, ID3D11DeviceContext* _context) {

	ID3D11Resource** texture = 0;
	CreateWICTextureFromFile(_device, _context, _filepath, texture, &resourceView);

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = _addressMode;
	samplerDesc.AddressV = _addressMode;
	samplerDesc.AddressW = _addressMode;
	samplerDesc.Filter = _filterMode;
	samplerDesc.MaxLOD = _maxLOD;
	_device->CreateSamplerState(&samplerDesc, &samplerState);

}

DimEngine::Rendering::Texture::~Texture() {
	if (resourceView) resourceView->Release();
	if (samplerState) samplerState->Release();
}