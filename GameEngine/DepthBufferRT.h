#pragma once
#include "WindowHelper.h"
#include <d3d11.h>
#include "Graphics.h"
#include "Bindable.h"
#include "WytEnum.h"

namespace Bind
{
	class DepthBufferRT : public Bindable
	{
	public:
		DepthBufferRT(Graphics& gfx, int textureWidth, int textureHeight);
		DepthBufferRT(const DepthBufferRT& rhs) = default;
		~DepthBufferRT() = default;

		void Bind(Graphics& gfx)noexcept override;

		void SetRenderTarget(Graphics& gfx) noexcept;
		void ClearDepth(Graphics& gfx);
		ID3D11ShaderResourceView* GetShaderResourceView();


	private:
		int texWidth;
		int texHeight;

		//DepthBuffer
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pDepthBackBufferTexture2D;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDephtStencilView;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV;
		D3D11_VIEWPORT m_ViewPort;


	};
}