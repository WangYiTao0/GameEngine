#include "RTT.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"
#include "StringHelper.h"
#include "Surface.h"

namespace Bind
{
	RTT::RTT(Graphics& gfx, int TextureWidth, int TexureHeight)
	{
		INFOMAN(gfx);

		//1  create 2d render texture
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		textureDesc.Width = TextureWidth;
		textureDesc.Height = TexureHeight;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(&textureDesc, NULL, &mRenderTargetTexture));

		//2Ccreate render target view
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		GFX_THROW_INFO(GetDevice(gfx)->CreateRenderTargetView(mRenderTargetTexture.Get(), &renderTargetViewDesc, mRenderTargetView.GetAddressOf()));


		//3 create shader resource view
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(mRenderTargetTexture.Get(), &shaderResourceViewDesc, mShaderResourceView.GetAddressOf()));

	}

	void RTT::Bind(Graphics& gfx) noexcept
	{
		SetRenderTarget(gfx);
		ClearRenderTarget(gfx);
	}

	std::shared_ptr<RTT> RTT::Resolve(Graphics& gfx, int TextureWidth, int TexureHeight)
	{
		return Codex::Resolve<RTT>(gfx,TextureWidth,TexureHeight);
	}

	std::string RTT::GenerateUID(int TextureWidth, int TexureHeight)
	{
		using namespace std::string_literals;
		//using path & slot
		return typeid(RTT).name() + "#"s + std::to_string(TextureWidth) + "#" + std::to_string(TexureHeight);
	}

	std::string RTT::GetUID() const noexcept
	{
		return GenerateUID(texWidth, texHeight);
	}

	void RTT::SetRenderTarget(Graphics& gfx)
	{
		GetContext(gfx)->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), gfx.GetDepthStencilView());
	}
	void RTT::ClearRenderTarget(Graphics& gfx)
	{
		auto color = gfx.GetClearColor();

		GetContext(gfx)->ClearRenderTargetView(mRenderTargetView.Get(), color);

		GetContext(gfx)->ClearDepthStencilView(gfx.GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	}
	ID3D11ShaderResourceView* RTT::GetShaderResourceView()
	{
		return mShaderResourceView.Get();
	}
}