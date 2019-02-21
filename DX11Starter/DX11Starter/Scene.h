#pragma once

#include <map>
#include <string>

#include "Renderable.h"
#include "Texture.h"

class Scene {

public:

	Scene();
	~Scene();

	void Init(ID3D11Device* _device, ID3D11DeviceContext* _context);

	Renderable* CreateObject(std::string _id, Mesh* _mesh, Material* _material);
	Renderable* GetGameObject(std::string _id);
	std::map<std::string, Renderable*> GetAllGameObjects() { return gameobjects; }

	Mesh* CreateMesh(std::string _id, Vertex* _vertices, int _vertexArrayLen, UINT* _indices, int _indexArrayLen);
	Mesh* CreateMesh(std::string _id, char* _objFilepath);
	Mesh* GetMesh(std::string _id);

	Texture* CreateTexture(std::string _id, wchar_t* _filepath, D3D11_TEXTURE_ADDRESS_MODE _addressMode, D3D11_FILTER _filterMode, float _maxLOD);
	Texture* GetTexture(std::string _id);

	Material* CreateMaterial(std::string _id, SimpleVertexShader* _vertexShader, SimplePixelShader* _pixelShader, Texture* _texture);
	Material* GetMaterial(std::string _id);

private:

	ID3D11Device* device;
	ID3D11DeviceContext* context;

	std::map<std::string, Renderable*> gameobjects;
	std::map<std::string, Mesh*> meshes;
	std::map<std::string, Texture*> textures;
	std::map<std::string, Material*> materials;

};