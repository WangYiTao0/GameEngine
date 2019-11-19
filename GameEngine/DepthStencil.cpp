#include "DepthStencil.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"
namespace Bind
{
	DepthStencil::DepthStencil(Graphics& gfx, DepthStencilState Dss)
		:
		Dss(Dss)
	{
		INFOMAN(gfx);

			D3D11_DEPTH_STENCIL_DESC dsDesc = {};

		switch (Dss)
		{
		case DepthStencilState::DSSWriteStencil:
			//mirror
			//do not write depth message
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
		
			break;
		case DepthStencilState::DSSDrawWithStencil:

			dsDesc.DepthEnable = true;
			dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

			dsDesc.StencilEnable = true;
			dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
			dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

			dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
			
			dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

			break;
		case DepthStencilState::DSSNoDoubleBlend:

			dsDesc.DepthEnable = true;
			dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

			dsDesc.StencilEnable = true;
			dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
			dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

			dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
			dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
			dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
			dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

			break;
		case DepthStencilState::DSSNoDepthTest:

			dsDesc.DepthEnable = false;
			dsDesc.StencilEnable = false;
			break;
		case DepthStencilState::DSSNoDepthWrite:

			dsDesc.DepthEnable = true;
			dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
			dsDesc.StencilEnable = false;
			break;
		default:
			break;
		}

		GFX_THROW_INFO(GetDevice(gfx)->CreateDepthStencilState(&dsDesc, DStencil.GetAddressOf()));

	}

	void Bind::DepthStencil::Bind(Graphics& gfx) noexcept
	{
	}

	std::shared_ptr<DepthStencil> Bind::DepthStencil::Resolve(Graphics& gfx, DepthStencilState Dss)
	{
		return Codex::Resolve<DepthStencil>(gfx, Dss);
	}

	std::string Bind::DepthStencil::GenerateUID(DepthStencilState Dss)
	{
		using namespace std::string_literals;
		return typeid(DepthStencil).name() + "#"s + std::to_string((int)Dss);
	}

	std::string Bind::DepthStencil::GetUID() const noexcept
	{
		return GenerateUID(this->Dss);
	}

}