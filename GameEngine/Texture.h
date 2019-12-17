#pragma once
#include "Bindable.h"
#include "Color.h"
#include <assimp/material.h>
#include <DirectXTK/WICTextureLoader.h>

namespace Bind
{
	enum class TextureStorageType
	{
		Invalid,
		None,
		EmbeddedIndexCompressed,
		EmbeddedIndexNonCompressed,
		EmbeddedCompressed,
		EmbeddedNonCompressed,
		Disk
	};

	class Texture : public Bindable
	{
	public:
		Texture(Graphics& gfx, const std::string& filePath, UINT slot = 0, ID3D11ShaderResourceView* pTv = nullptr);
		Texture(Graphics& gfx, std::vector<std::string>& filePaths, UINT slot = 0);
		Texture(Graphics& gfx, std::string& filePaths, UINT slot = 0, bool generateMips = true);
		void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<Texture> Resolve(Graphics& gfx, const std::string& filePath, UINT slot = 0, ID3D11ShaderResourceView* pTv = nullptr);
		static std::string GenerateUID(const std::string& filePath, UINT slot = 0, ID3D11ShaderResourceView* pTv = nullptr);
		std::string GetUID() const noexcept override;
		bool HasAlpha() const noexcept;
		//Texture(Graphics& gfx, const uint8_t* pData, size_t size, aiTextureType type);
	private:
		void Initialize1x1ColorTexture(Graphics& gfx,const Color& colorData, aiTextureType type);
		void InitializeColorTexture(Graphics& gfx,const Color* colorData, UINT width, UINT height, aiTextureType type);
		
	private:
		unsigned int slot;
	protected:
		bool hasAlpha = false;
		std::string filePath;
		Microsoft::WRL::ComPtr<ID3D11Resource> pSrcTex = nullptr;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pSRV = nullptr;
		//Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture2D =nullptr;
		aiTextureType type = aiTextureType::aiTextureType_UNKNOWN;
	};
}
