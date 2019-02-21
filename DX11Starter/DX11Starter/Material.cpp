#include "Material.h"

Material::Material(SimpleVertexShader* _vertexShader, SimplePixelShader* _pixelShader, Texture* _texture) {
	vertexShader = _vertexShader;
	pixelShader = _pixelShader;
	shaderResourceView = _texture->GetResourceView();
	samplerState = _texture->GetSamplerState();
}

Material::~Material() {

}