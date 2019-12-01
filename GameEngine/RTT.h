#pragma once
#include "WindowHelper.h"
#include <d3d11.h>
#include <wrl/client.h>
#include "Graphics.h"
#include "Bindable.h"

namespace Bind
{
	class RTT : public Bindable
	{
	public:
		RTT(Graphics& gfx, int TextureWidth, int TexureHeight);
		RTT(const RTT& rhs) = default;
		~RTT() = default;

		void SetRenderTarget(Graphics& gfx, ID3D11DepthStencilView* depthStencilView);
		void ClearRenderTarget(Graphics& gfx, ID3D11DepthStencilView* depthStencilView, float red, float green, float blue, float alpha);
		ID3D11ShaderResourceView* GetShaderResourceView();

	private:

		Microsoft::WRL::ComPtr<ID3D11Texture2D> mRenderTargetTexture;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mShaderResourceView;
	};
}