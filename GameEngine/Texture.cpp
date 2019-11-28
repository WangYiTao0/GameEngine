#include "Texture.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"
#include "StringHelper.h"
#include <DirectXTK\WICTextureLoader.h>
#include <DirectXTK\DDSTextureLoader.h>


namespace Bind
{
	namespace wrl = Microsoft::WRL;


	Texture::Texture(Graphics& gfx, const std::string& filePath, UINT slot,aiTextureType type)
		:
		path(path),
		slot(slot),
		type(type)
	{
		INFOMAN(gfx);

		if (StringHelper::GetFileExtension(filePath) == ".dds")
		{
			HRESULT hr = DirectX::CreateDDSTextureFromFileEx(
				GetDevice(gfx),
				GetContext(gfx),// if given a d3dContext instance for auto-gen mipmap support. 
				StringHelper::StringToWide(filePath).c_str(),
				0,
				D3D11_USAGE_DEFAULT,
				D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
				0,
				D3D11_RESOURCE_MISC_GENERATE_MIPS,
				0,
				pTexture.GetAddressOf(), this->pTextureView.GetAddressOf());
			if (FAILED(hr))
			{
				Initialize1x1ColorTexture(gfx,Colors::UnloadedTextureColor, type);
			}
			return;
		}
		else
		{
			HRESULT hr = DirectX::CreateWICTextureFromFileEx(
				GetDevice(gfx),
				GetContext(gfx),// if given a d3dContext instance for auto-gen mipmap support. 
				StringHelper::StringToWide(filePath).c_str(),
				0, 
				D3D11_USAGE_DEFAULT, 
				D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
				0, 
				D3D11_RESOURCE_MISC_GENERATE_MIPS, 
				0,
				pTexture.GetAddressOf(),
				pTextureView.GetAddressOf());
			if (FAILED(hr))
			{
				Initialize1x1ColorTexture(gfx,Colors::UnloadedTextureColor, type);
			}
			return;
		}
	
	}

	Texture::Texture(Graphics& gfx,const uint8_t* pData, size_t size, aiTextureType type)
	{
		this->type = type;
		HRESULT hr = DirectX::CreateWICTextureFromMemory(GetDevice(gfx), pData, size, this->pTexture.GetAddressOf(), this->pTextureView.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create Texture from memory.");
	}

	void Texture::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->PSSetShaderResources(slot, 1u, pTextureView.GetAddressOf());
	}

	std::shared_ptr<Texture> Texture::Resolve(Graphics& gfx, const std::string& path, UINT slot)
	{
		return Codex::Resolve<Texture>(gfx, path, slot);
	}
	std::string Texture::GenerateUID(const std::string& path, UINT slot)
	{
		using namespace std::string_literals;
		//using path & slot
		return typeid(Texture).name() + "#"s + path + "#" + std::to_string(slot);
	}
	std::string Texture::GetUID() const noexcept
	{
		return GenerateUID(path, slot);
	}
	bool Texture::HasAlpha() const noexcept
	{
		return hasAlpha;
	}

	void Texture::Initialize1x1ColorTexture(Graphics& gfx,const Color& colorData, aiTextureType type)
	{
		InitializeColorTexture(gfx,&colorData, 1, 1, type);
	}
	
	void Texture::InitializeColorTexture(Graphics& gfx,const Color* colorData, UINT width, UINT height, aiTextureType type)
	{
		this->type = type;
		CD3D11_TEXTURE2D_DESC textureDesc(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
		ID3D11Texture2D* p2DTexture = nullptr;
		D3D11_SUBRESOURCE_DATA initialData{};
		initialData.pSysMem = colorData;
		initialData.SysMemPitch = width * sizeof(Color);
		HRESULT hr = GetDevice(gfx)->CreateTexture2D(&textureDesc, &initialData, &p2DTexture);
		COM_ERROR_IF_FAILED(hr, "Failed to initialize texture from color data.");
		pTexture = static_cast<ID3D11Texture2D*>(p2DTexture);
		CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format);
		hr = GetDevice(gfx)->CreateShaderResourceView(pTexture.Get(), &srvDesc, pTextureView.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create shader resource view from texture generated from color data.");
	}
}
