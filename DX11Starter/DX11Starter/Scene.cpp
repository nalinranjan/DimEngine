#include "Scene.h"

#include <iostream>
#include "WICTextureLoader.h"

using namespace DirectX;

Scene::Scene() {

}

Scene::~Scene() {

	for (const auto &element : gameobjects) {
		Renderable* obj = element.second;
		if (obj) {
			delete obj;
		}
	}
	gameobjects.clear();

	for (const auto &element : meshes) {
		Mesh* obj = element.second;
		if (obj) {
			delete obj;
		}
	}
	meshes.clear();

	for (const auto &element : textures) {
		Texture* obj = element.second;
		if (obj) {
			delete obj;
		}
	}
	meshes.clear();

	for (const auto &element : materials) {
		Material* obj = element.second;
		if (obj) {
			delete obj;
		}
	}
	materials.clear();

}

void Scene::Init(ID3D11Device* _device, ID3D11DeviceContext* _context) {
	device = _device;
	context = _context;
}

Renderable* Scene::CreateObject(std::string _id, Mesh* _mesh, Material* _material) {

	if (gameobjects.find(_id) == gameobjects.end()) {
		Renderable* obj = new Renderable(_mesh, _material);
		gameobjects.insert(std::pair<std::string, Renderable*>(_id, obj));
		return obj;
	}
	else {
		std::cerr << "Could not create Object. Id '" << _id << "' already exists." << std::endl;
		return 0;
	}

}

Renderable* Scene::GetGameObject(std::string _id) {
	try {
		return gameobjects.at(_id);
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Could not find Object with Id '" << _id << "'." << std::endl;
	}
	return 0;
}

Mesh* Scene::CreateMesh(std::string _id, Vertex* _vertices, int _vertexArrayLen, UINT* _indices, int _indexArrayLen) {

	if (meshes.find(_id) == meshes.end()) {
		Mesh* mesh = new Mesh(_vertices, _vertexArrayLen, _indices, _indexArrayLen, device);
		meshes.insert(std::pair<std::string, Mesh*>(_id, mesh));
		return mesh;
	}
	else {
		std::cerr << "Could not create Mesh. Id '" << _id << "' already exists." << std::endl;
		return 0;
	}

}

Mesh* Scene::CreateMesh(std::string _id, char* _objFilepath) {

	if (meshes.find(_id) == meshes.end()) {
		Mesh* mesh = new Mesh(_objFilepath, device);
		meshes.insert(std::pair<std::string, Mesh*>(_id, mesh));
		return mesh;
	}
	else {
		std::cerr << "Could not create Mesh. Id '" << _id << "' already exists." << std::endl;
		return 0;
	}

}

Mesh* Scene::GetMesh(std::string _id) {
	try {
		return meshes.at(_id);
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Could not find Mesh with Id '" << _id << "'." << std::endl;
	}
	return 0;
}

Texture* Scene::CreateTexture(std::string _id, wchar_t* _filepath, D3D11_TEXTURE_ADDRESS_MODE _addressMode, D3D11_FILTER _filterMode, float _maxLOD) {

	if (textures.find(_id) == textures.end()) {
		Texture* tex = new Texture(_filepath, _addressMode, _filterMode, _maxLOD, device, context);
		textures.insert(std::pair<std::string, Texture*>(_id, tex));
		return tex;
	}
	else {
		std::cerr << "Could not create Texture. Id '" << _id << "' already exists." << std::endl;
		return 0;
	}

}

Texture* Scene::GetTexture(std::string _id) {
	try {
		return textures.at(_id);
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Could not find Texture with Id '" << _id << "'." << std::endl;
	}
	return 0;
}

Material* Scene::CreateMaterial(std::string _id, SimpleVertexShader* _vertexShader, SimplePixelShader* _pixelShader, Texture* _texture) {

	if (materials.find(_id) == materials.end()) {
		Material* mat = new Material(_vertexShader, _pixelShader, _texture);
		materials.insert(std::pair<std::string, Material*>(_id, mat));
		return mat;
	}
	else {
		std::cerr << "Could not create Material. Id '" << _id << "' already exists." << std::endl;
		return 0;
	}

}

Material* Scene::GetMaterial(std::string _id) {
	try {
		return materials.at(_id);
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Could not find Material with Id '" << _id << "'." << std::endl;
	}
	return 0;
}