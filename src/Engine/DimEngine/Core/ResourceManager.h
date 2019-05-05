#pragma once

#include "../Rendering/Mesh.h"
#include "../Rendering/Texture.h"
#include "../Rendering/Material.h"
#include <map>
#include <string>

namespace DimEngine {

	class __declspec(dllexport) ResourceManager {

	private:

		static ResourceManager* singleton;

		ID3D11Device* device;
		ID3D11DeviceContext* context;

		std::map<std::string, Rendering::Mesh*> meshes;
		std::map<std::string, Rendering::Texture*> textures;
		std::map<std::string, Rendering::Material*> materials;

	public:

		static ResourceManager* GetSingleton();

		ResourceManager();
		~ResourceManager();
		void Initialize(ID3D11Device* _device, ID3D11DeviceContext* _context);

		Rendering::Mesh* CreateMesh(std::string _id, Rendering::Vertex* _vertices, int _vertexArrayLen, UINT* _indices, int _indexArrayLen);
		Rendering::Mesh* CreateMesh(std::string _id, const char* _objFilepath);
		Rendering::Mesh* GetMesh(std::string _id);

		Rendering::Texture* CreateTexture(std::string _id, wchar_t* _filepath, D3D11_TEXTURE_ADDRESS_MODE _addressMode, D3D11_FILTER _filterMode, float _maxLOD);
		Rendering::Texture* GetTexture(std::string _id);

		Rendering::Material* CreateMaterial(std::string _id, SimpleVertexShader* _vertexShader, SimplePixelShader* _pixelShader, ID3D11SamplerState* _sampler);
		Rendering::Material* GetMaterial(std::string _id);

	};

}

