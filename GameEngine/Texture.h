#pragma once
#include "Bindable.h"
#include "Color.h"
#include <assimp/material.h>

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
		Texture(Graphics& gfx, const std::string& filePath, UINT slot = 0, aiTextureType type = aiTextureType_UNKNOWN);
		void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<Texture> Resolve(Graphics& gfx, const std::string& path, UINT slot = 0);
		static std::string GenerateUID(const std::string& path, UINT slot = 0);
		std::string GetUID() const noexcept override;
		bool HasAlpha() const noexcept;
		Texture(Graphics& gfx, const uint8_t* pData, size_t size, aiTextureType type);
	private:
		void Initialize1x1ColorTexture(Graphics& gfx,const Color& colorData, aiTextureType type);
		void InitializeColorTexture(Graphics& gfx,const Color* colorData, UINT width, UINT height, aiTextureType type);
		
	private:
		unsigned int slot;
	protected:
		bool hasAlpha = false;
		std::string path;
		Microsoft::WRL::ComPtr<ID3D11Resource> pTexture = nullptr;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView = nullptr;
		//Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture2D =nullptr;
		aiTextureType type = aiTextureType::aiTextureType_UNKNOWN;
	};
}
