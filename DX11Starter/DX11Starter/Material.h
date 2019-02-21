#pragma once

#include "SimpleShader.h"
#include "Texture.h"

class Material {

public:

	Material(SimpleVertexShader* _vertexShader, SimplePixelShader* _pixelShader, Texture* _texture);
	~Material();

	SimpleVertexShader* GetVertexShader() { return vertexShader; }
	SimplePixelShader* GetPixelShader() { return pixelShader; }
	ID3D11ShaderResourceView* GetShaderResourceView() { return shaderResourceView; }
	ID3D11SamplerState* GetSamplerState() { return samplerState; }

private:

	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	ID3D11ShaderResourceView* shaderResourceView;
	ID3D11SamplerState* samplerState;

};