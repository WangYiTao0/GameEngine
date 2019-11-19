#include "DepthStencil.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"
namespace Bind
{
	DepthStencil::DepthStencil(Graphics& gfx)
	{
		INFOMAN(gfx);

		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

		dsDesc.StencilEnable = true;
		dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		GFX_THROW_INFO(GetDevice(gfx)->CreateDepthStencilState(&dsDesc, DSSWriteStencil.GetAddressOf()));


	}

	void Bind::DepthStencil::Bind(Graphics& gfx) noexcept
	{
	}

	std::shared_ptr<DepthStencil> Bind::DepthStencil::Resolve(Graphics& gfx)
	{
		return std::shared_ptr<DepthStencil>();
	}

	std::string Bind::DepthStencil::GenerateUID()
	{
		return std::string();
	}

	std::string Bind::DepthStencil::GetUID() const noexcept
	{
		return std::string();
	}

}