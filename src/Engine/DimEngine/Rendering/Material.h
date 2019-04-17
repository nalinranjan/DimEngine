#pragma once

#include "SimpleShader.h"
#include "Texture.h"

#include <unordered_map>
#include <string>

namespace DimEngine
{
	namespace Rendering
	{
		struct __declspec(dllexport) Material
		{
		public:
			Material(
				SimpleVertexShader* _vertexShader,
				SimplePixelShader* _pixelShader,
				ID3D11ShaderResourceView* _texture,
				ID3D11SamplerState* _sampler
			);

			Material(
				SimpleVertexShader* _vertexShader,
				SimplePixelShader* _pixelShader,
				Texture* texture
			);

			SimpleVertexShader* GetVertexShader();
			SimplePixelShader* GetPixelShader();

			ID3D11ShaderResourceView* getTexture();
			ID3D11SamplerState* getSampler();

			std::unordered_map<std::string, std::pair<const void*, unsigned int>> GetVertexShaderData() { return vertexShaderData; }
			std::unordered_map<std::string, std::pair<const void*, unsigned int>> GetPixelShaderData() { return pixelShaderData; }

			void SetVertexData(std::string name, const void* data, unsigned int size);
			void SetPixelData(std::string name, const void* data, unsigned int size);

		private:
			SimpleVertexShader* vertexShader;
			SimplePixelShader* pixelShader;

			ID3D11ShaderResourceView* texture;
			ID3D11SamplerState* sampler;

			std::unordered_map<std::string, std::pair<const void*, unsigned int>> vertexShaderData;
			std::unordered_map<std::string, std::pair<const void*, unsigned int>> pixelShaderData;

		};
	}
}
