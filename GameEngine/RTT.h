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

		void Bind(Graphics& gfx)noexcept override;

		static std::shared_ptr<RTT> Resolve(Graphics& gfx, int TextureWidth, int TexureHeight);
		static std::string GenerateUID(int TextureWidth, int TexureHeight);
		std::string GetUID() const noexcept override;


		void SetRenderTarget(Graphics& gfx);
		void ClearRenderTarget(Graphics& gfx);
		ID3D11ShaderResourceView* GetShaderResourceView();

	private:
		int texWidth;
		int texHeight;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> mRenderTargetTexture;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mShaderResourceView;
	};
}