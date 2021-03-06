﻿#include "RTT.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"
#include "StringHelper.h"
#include "Surface.h"

namespace Bind
{
	RTT::RTT(Graphics& gfx, int textureWidth, int textureHeight, TextureFormat eTextureFormat)
		:
		texWidth(textureWidth),
		texHeight(textureHeight)
	{
		INFOMAN(gfx);

	
		D3D11_TEXTURE2D_DESC textureDesc;

		D3D11_TEXTURE2D_DESC depthStencilDesc;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;	
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	

		switch (eTextureFormat)
		{
		case R32G32B32A32:
			textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		case R32:
			textureDesc.Format = DXGI_FORMAT_R32_FLOAT;
			break;
		default:
			break;
		}
		//1  create 2d render texture
		textureDesc.Width = textureWidth;
		textureDesc.Height = textureHeight;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(&textureDesc, NULL, &m_pBackBufferTexture));

		//2，create render target view
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		GFX_THROW_INFO(GetDevice(gfx)->CreateRenderTargetView(m_pBackBufferTexture.Get(), &renderTargetViewDesc, m_pRTV.GetAddressOf()));


		//3 create shader resource view
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(m_pBackBufferTexture.Get(), &shaderResourceViewDesc, m_pShaderResourceView.GetAddressOf()));

		//4 create depthstencilDesc
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		//depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		depthStencilDesc.Width = textureWidth;
		depthStencilDesc.Height = textureHeight;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;
		GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(&depthStencilDesc,0,&m_pDepthBackBufferTexture2D));

		//create depthStencilViewDesc
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		depthStencilViewDesc.Format = depthStencilDesc.Format;

		GFX_THROW_INFO(GetDevice(gfx)->CreateDepthStencilView(
			m_pDepthBackBufferTexture2D.Get(), 
			&depthStencilViewDesc,
			m_pDSV.GetAddressOf()));

		//create view Port
		m_ViewPort.Width = static_cast<float>(textureWidth);
		m_ViewPort.Height = static_cast<float>(textureHeight);
		m_ViewPort.MinDepth = 0.0f;
		m_ViewPort.MaxDepth = 1.0f;
		m_ViewPort.TopLeftX = 0.0f;
		m_ViewPort.TopLeftY = 0.0f;

	}

	void RTT::Bind(Graphics& gfx) noexcept
	{
		SetRenderTarget(gfx);
		ClearRenderTarget(gfx);
	}

	void RTT::SetRenderTarget(Graphics& gfx)
	{
		GetContext(gfx)->OMSetRenderTargets(1, m_pRTV.GetAddressOf(), m_pDSV.Get());
		GetContext(gfx)->RSSetViewports(1, &m_ViewPort);

		ClearRenderTarget(gfx);
		ClearDepthStencilBuffer(gfx);
	}
	void RTT::ClearDepthStencilBuffer(Graphics& gfx)
	{
		GetContext(gfx)->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
	void RTT::ClearRenderTarget(Graphics& gfx)
	{
		auto color = gfx.GetClearColor();

		GetContext(gfx)->ClearRenderTargetView(m_pRTV.Get(), color);
	}
	ID3D11ShaderResourceView* RTT::GetShaderResourceView()
	{
		return m_pShaderResourceView.Get();
	}
	ID3D11RenderTargetView* RTT::GetRenderTargetView()
	{
		return m_pRTV.Get();
	}

	//std::shared_ptr<RTT> RTT::Resolve(Graphics& gfx, int TextureWidth, int TexureHeight)
	//{
	//	return Codex::Resolve<RTT>(gfx,TextureWidth,TexureHeight);
	//}

	//std::string RTT::GenerateUID(int TextureWidth, int TexureHeight)
	//{
	//	using namespace std::string_literals;
	//	//using path & slot
	//	return typeid(RTT).name() + "#"s + std::to_string(TextureWidth) + "#" + std::to_string(TexureHeight);
	//}

	//std::string RTT::GetUID() const noexcept
	//{
	//	return GenerateUID(texWidth, texHeight);
	//}
}