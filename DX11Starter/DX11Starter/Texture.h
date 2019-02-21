#pragma once

#include "WICTextureLoader.h"

class Texture {

public:

	Texture(wchar_t* _filepath, D3D11_TEXTURE_ADDRESS_MODE _addressMode, D3D11_FILTER _filterMode, float _maxLOD, ID3D11Device* _device, ID3D11DeviceContext* _context);
	~Texture();

	ID3D11ShaderResourceView* GetResourceView() { return resourceView; }
	ID3D11SamplerState* GetSamplerState() { return samplerState; }

private:

	ID3D11ShaderResourceView* resourceView;
	ID3D11SamplerState* samplerState;

};