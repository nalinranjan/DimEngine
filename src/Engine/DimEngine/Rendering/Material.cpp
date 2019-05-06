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

	normalMap = 0;
	//shadowMap = 0;
	metalnessMap = 0;
	roughnessMap = 0;

	hasTexture = 0;
	hasNormalMap = 0;
	hasShadowMap = 0;
	hasMetalnessMap = 0;
	hasRoughnessMap = 0;
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

	normalMap = 0;
	//shadowMap = 0;
	metalnessMap = 0;
	roughnessMap = 0;

	hasTexture = 0;
	hasNormalMap = 0;
	hasShadowMap = 0;
	hasMetalnessMap = 0;
	hasRoughnessMap = 0;
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

void DimEngine::Rendering::Material::setTexture(ID3D11ShaderResourceView * _texture) {
	texture = _texture;
	hasTexture = 1;
}

void DimEngine::Rendering::Material::setNormalMap(ID3D11ShaderResourceView * _normalMap) {
	normalMap = _normalMap;
	hasNormalMap = 1;
}

void DimEngine::Rendering::Material::setShadowMap(ShadowMap* _shadowMap) {
	shadowMap = _shadowMap;
	hasShadowMap = 1;
}

void DimEngine::Rendering::Material::setMetalnessMap(ID3D11ShaderResourceView * _metalnessMap) {
	metalnessMap = _metalnessMap;
	hasMetalnessMap = 1;
}

void DimEngine::Rendering::Material::setRoughnessMap(ID3D11ShaderResourceView * _roughnessMap) {
	roughnessMap = _roughnessMap;
	hasRoughnessMap = 1;
}