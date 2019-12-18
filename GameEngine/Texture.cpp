#include "Texture.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"
#include "StringHelper.h"
#include "Surface.h"
#include "d3dUtil.h"

namespace Bind
{
	namespace wrl = Microsoft::WRL;

	Texture::Texture(Graphics& gfx, const std::string& filePath,UINT slot, ID3D11ShaderResourceView* pTv)
		:
		filePath(filePath),
		slot(slot),
		type(type),
		pSRV(pTv)
	{
		INFOMAN(gfx);

		if (filePath !="null")
		{
			// load surface
			const auto s = Surface::FromFile(filePath);
			hasAlpha = s.AlphaLoaded();

			// create texture resource
			D3D11_TEXTURE2D_DESC textureDesc = {};
			textureDesc.Width = s.GetWidth();
			textureDesc.Height = s.GetHeight();
			textureDesc.MipLevels = 0;
			textureDesc.ArraySize = 1;
			textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
			wrl::ComPtr<ID3D11Texture2D> pTexture;
			GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(
				&textureDesc, nullptr, &pTexture
			));

			// write image data into top mip level
			GetContext(gfx)->UpdateSubresource(
				pTexture.Get(), 0u, nullptr, s.GetBufferPtrConst(), s.GetWidth() * sizeof(Surface::Color), 0u
			);

			// create the resource view on the texture
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Format = textureDesc.Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = -1;
			GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(
				pTexture.Get(), &srvDesc, &pSRV
			));

			// generate the mip chain using the gpu rendering pipeline
			GetContext(gfx)->GenerateMips(pSRV.Get());
		}
		else
		{
			pSRV = pTv;
		}
	}

	Texture::Texture(Graphics& gfx, std::vector<std::string>& filePaths, UINT slot )
		:
		slot(slot)
	{
		INFOMAN(gfx);
		std::vector<std::wstring> wFilePaths;

		for (auto it : filePaths)
		{
			wFilePaths.push_back(StrH::ToWide(it));
		}

	
		GFX_THROW_INFO(CreateWICTexture2DCubeFromFile(
			GetDevice(gfx),
			GetContext(gfx),
			wFilePaths,
			nullptr,
			pSRV.GetAddressOf(),
			true
		));
	}


	void Texture::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->PSSetShaderResources(slot, 1u, pSRV.GetAddressOf());
	}

	std::shared_ptr<Texture> Texture::Resolve(Graphics& gfx, const std::string& filePath, UINT slot, ID3D11ShaderResourceView* pTv)
	{
		return Codex::Resolve<Texture>(gfx, filePath, slot, pTv);
	}
	std::string Texture::GenerateUID(const std::string& filePath, UINT slot, ID3D11ShaderResourceView* pTv)
	{
		using namespace std::string_literals;
		//using path & slot
		return typeid(Texture).name() + "#"s + filePath + "#" + std::to_string(slot) + "#" + typeid(ID3D11ShaderResourceView).name();
	}
	std::string Texture::GetUID() const noexcept
	{
		return GenerateUID(filePath, slot, pSRV.Get());
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
		pSrcTex = static_cast<ID3D11Texture2D*>(p2DTexture);
		CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format);
		hr = GetDevice(gfx)->CreateShaderResourceView(pSrcTex.Get(), &srvDesc, pSRV.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create shader resource view from texture generated from color data.");
	}
}
