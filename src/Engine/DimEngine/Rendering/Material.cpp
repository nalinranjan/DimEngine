#include "Material.h"

DimEngine::Rendering::Material::Material(
	SimpleVertexShader* _vertexShader,
	SimplePixelShader* _pixelShader,
	ID3D11ShaderResourceView* _texture,
	ID3D11SamplerState* _sampler
) {
	vertexShader = _vertexShader;
	pixelShader = _pixelShader;
	texture = _texture;
	sampler = _sampler;
}

SimpleVertexShader* DimEngine::Rendering::Material::GetVertexShader() {
	return vertexShader;
}

SimplePixelShader* DimEngine::Rendering::Material::GetPixelShader() {
	return pixelShader;
}

ID3D11ShaderResourceView* DimEngine::Rendering::Material::getTexture() {
	return texture;
}

ID3D11SamplerState* DimEngine::Rendering::Material::getSampler() {
	return sampler;
}