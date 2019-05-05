#include "ResourceManager.h"

#include <iostream>

using namespace DimEngine;

ResourceManager* ResourceManager::singleton = nullptr;

ResourceManager* ResourceManager::GetSingleton() {
	if (!singleton) singleton = new ResourceManager();
	return singleton;
}

void ResourceManager::Initialize(ID3D11Device* _device, ID3D11DeviceContext* _context) {
	device = _device;
	context = _context;
}

ResourceManager::ResourceManager() {
	
}

ResourceManager::~ResourceManager() {

	for (const auto &element : meshes) {
		Rendering::Mesh* obj = element.second;
		if (obj) {
			delete obj;
		}
	}
	meshes.clear();

	for (const auto &element : textures) {
		Rendering::Texture* obj = element.second;
		if (obj) {
			delete obj;
		}
	}
	textures.clear();

}

// MESHES

Rendering::Mesh* ResourceManager::CreateMesh(std::string _id, DimEngine::Rendering::Vertex* _vertices, int _vertexArrayLen, UINT* _indices, int _indexArrayLen) {
	if (meshes.find(_id) == meshes.end()) {
		Rendering::Mesh* mesh = new Rendering::Mesh(_vertices, _vertexArrayLen, _indices, _indexArrayLen, device);
		meshes.insert(std::pair<std::string, Rendering::Mesh*>(_id, mesh));
		return mesh;
	}
	else {
		std::cerr << "Could not create Mesh. Id '" << _id << "' already exists." << std::endl;
		return 0;
	}
}

Rendering::Mesh* DimEngine::ResourceManager::CreateMesh(std::string _id, const char* _objFilepath) {
	if (meshes.find(_id) == meshes.end()) {
		Rendering::Mesh* mesh = new DimEngine::Rendering::Mesh(device, _objFilepath);
		meshes.insert(std::pair<std::string, DimEngine::Rendering::Mesh*>(_id, mesh));
		return mesh;
	}
	else {
		std::cerr << "Could not create Mesh. Id '" << _id << "' already exists." << std::endl;
		return 0;
	}
}

Rendering::Mesh* ResourceManager::GetMesh(std::string _id) {
	try {
		return meshes.at(_id);
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Could not find Mesh with Id '" << _id << "'." << std::endl;
	}
	return 0;
}

// TEXTURES

Rendering::Texture* ResourceManager::CreateTexture(std::string _id, wchar_t* _filepath, D3D11_TEXTURE_ADDRESS_MODE _addressMode, D3D11_FILTER _filterMode, float _maxLOD) {

	if (textures.find(_id) == textures.end()) {
		Rendering::Texture* tex = new Rendering::Texture(_filepath, _addressMode, _filterMode, _maxLOD);
		textures.insert(std::pair<std::string, Rendering::Texture*>(_id, tex));
		return tex;
	}
	else {
		std::cerr << "Could not create Texture. Id '" << _id << "' already exists." << std::endl;
		return 0;
	}

}

Rendering::Texture* ResourceManager::GetTexture(std::string _id) {
	try {
		return textures.at(_id);
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Could not find Texture with Id '" << _id << "'." << std::endl;
	}
	return 0;
}

// MATERIALS

Rendering::Material* ResourceManager::CreateMaterial(std::string _id, SimpleVertexShader* _vertexShader, SimplePixelShader* _pixelShader, ID3D11SamplerState* _sampler) {

	if (materials.find(_id) == materials.end()) {
		Rendering::Material* mat = new Rendering::Material(_vertexShader, _pixelShader, _sampler);
		materials.insert(std::pair<std::string, Rendering::Material*>(_id, mat));
		return mat;
	}
	else {
		std::cerr << "Could not create Material. Id '" << _id << "' already exists." << std::endl;
		return 0;
	}

}

Rendering::Material* ResourceManager::GetMaterial(std::string _id) {
	try {
		return materials.at(_id);
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Could not find Material with Id '" << _id << "'." << std::endl;
	}
	return 0;
}