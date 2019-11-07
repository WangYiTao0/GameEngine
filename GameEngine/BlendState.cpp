#include "BlendState.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"

namespace Bind
{
	BlendState::BlendState(Graphics& gfx)
	{
		INFOMAN(gfx);

		//Create Blend State
		D3D11_RENDER_TARGET_BLEND_DESC rtbd = { 0 };
		rtbd.BlendEnable = true;
		rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

		D3D11_BLEND_DESC blendDesc = { 0 };
		blendDesc.RenderTarget[0] = rtbd;

		GFX_THROW_INFO(GetDevice(gfx)->CreateBlendState(&blendDesc, pBlendState.GetAddressOf()));
	}

	void BlendState::Bind(Graphics& gfx) noexcept
	{
		float BlendFactor[4] = { 0.0f,0.0f,0.0f,0.0f };
		GetContext(gfx)->OMSetBlendState(pBlendState.Get(), NULL, 0xffffffff);
	}

	std::shared_ptr<BlendState> BlendState::Resolve(Graphics& gfx)
	{
		return Codex::Resolve<BlendState>(gfx);
	}

	std::string BlendState::GenerateUID()
	{
		return typeid(BlendState).name();
	}

	std::string BlendState::GetUID() const noexcept
	{
		return  GenerateUID();
	}

}