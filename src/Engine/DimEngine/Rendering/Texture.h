#pragma once

#include <WICTextureLoader.h>

#include "../Common/Typedefs.h"

namespace DimEngine {
	namespace Rendering {

		class __declspec(dllexport) Texture {
		protected:
			ID3D11ShaderResourceView* resourceView;
			ID3D11SamplerState* samplerState;

			Texture();


		public:
			Texture(wchar_t* _filepath, D3D11_TEXTURE_ADDRESS_MODE _addressMode, D3D11_FILTER _filterMode, f32 _maxLOD, ID3D11Device* _device, ID3D11DeviceContext* _context);
			~Texture();

			ID3D11ShaderResourceView* GetResourceView() { return resourceView; }
			ID3D11SamplerState* GetSamplerState() { return samplerState; }
		};


		class __declspec(dllexport) RenderTexture : public Texture
		{
		private:
			ID3D11RenderTargetView* renderTargetView;
			ID3D11DepthStencilView* depthStencilView;
			u32 height;
			u32 width;


		public:
			RenderTexture(ID3D11Device* device, u32 size);
			RenderTexture(ID3D11Device* device, u32 height, u32 width);
			~RenderTexture();

			ID3D11RenderTargetView* GetRenderTargetView();
			ID3D11DepthStencilView* GetDepthStencilView();
		};
	}
}