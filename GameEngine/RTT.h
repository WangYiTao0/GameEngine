#pragma once
#include "WindowHelper.h"
#include <d3d11.h>
#include <wrl/client.h>
#include "Graphics.h"
#include "Bindable.h"
#include "WytEnum.h"

namespace Bind
{
	class RTT : public Bindable
	{
	public:
		RTT(Graphics& gfx, int textureWidth, int textureHeight, TextureFormat eTextureFormat = TextureFormat::R32G32B32A32);
		RTT(const RTT& rhs) = default;
		~RTT() = default;

		void Bind(Graphics& gfx)noexcept override;

		//static std::shared_ptr<RTT> Resolve(Graphics& gfx, int TextureWidth, int TexureHeight);
		//static std::string GenerateUID(int TextureWidth, int TexureHeight);
		//std::string GetUID() const noexcept override;

		void SetRenderTarget(Graphics& gfx);
		void ClearDepthStencilBuffer(Graphics& gfx);
		void ClearRenderTarget(Graphics& gfx);
		ID3D11ShaderResourceView* GetShaderResourceView();
		ID3D11RenderTargetView* GetRenderTargetView();

	private:
		int texWidth;
		int texHeight;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pBackBufferTexture;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRTV;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceView;

		//DepthBuffer
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pDepthBackBufferTexture2D;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDSV;
		D3D11_VIEWPORT m_ViewPort;


	};
}