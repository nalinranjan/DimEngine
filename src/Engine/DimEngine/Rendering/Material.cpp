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

DimEngine::Rendering::Material::Material(
	SimpleVertexShader* _vertexShader,
	SimplePixelShader* _pixelShader,
	Texture* texture
) {
	vertexShader = _vertexShader;
	pixelShader = _pixelShader;
	this->texture = texture->GetResourceView();
	this->sampler = texture->GetSamplerState();
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

void DimEngine::Rendering::Material::SetVertexData(std::string name, const void* data, unsigned int size) {
	vertexShaderData[name] = std::make_pair(data, size);
}

void DimEngine::Rendering::Material::SetPixelData(std::string name, const void* data, unsigned int size) {
	pixelShaderData[name] = std::make_pair(data, size);
}