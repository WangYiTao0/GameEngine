#include "Texture.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"
#include "StringHelper.h"
#include <DirectXTK\WICTextureLoader.h>
#include <DirectXTK\DDSTextureLoader.h>

namespace Bind
{
	namespace wrl = Microsoft::WRL;


	Texture::Texture(Graphics& gfx, const std::string& filePath, UINT slot)
		:
		path(path),
		slot(slot)
	{
		INFOMAN(gfx);

		ID3D11Texture2D* pTexture2D = nullptr;

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
				//this->Initialize1x1ColorTexture(device, Colors::UnloadedTextureColor, type);
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
				//this->Initialize1x1ColorTexture(device, Colors::UnloadedTextureColor, type);
			}
			return;
		}
	
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
	UINT Texture::CalculateNumberOfMipLevels(UINT width, UINT height) noexcept
	{
		const float xSteps = std::ceil(log2((float)width));
		const float ySteps = std::ceil(log2((float)height));
		return (UINT)std::max(xSteps, ySteps);
	}
}
