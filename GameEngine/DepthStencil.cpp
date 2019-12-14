#include "DepthStencil.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"
namespace Bind
{
	DepthStencil::DepthStencil(Graphics& gfx, Mode Dss)
		:
		Dss(Dss)
	{
		INFOMAN(gfx);

		D3D11_DEPTH_STENCIL_DESC dsDesc = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };

		switch (Dss)
		{
		case Mode::DSSWrite:
			dsDesc.StencilEnable = TRUE;
			dsDesc.StencilWriteMask = 0xFF;
			dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
			break;
		case Mode::DSSMask:
			dsDesc.DepthEnable = FALSE;
			dsDesc.StencilEnable = TRUE;
			dsDesc.StencilReadMask = 0xFF;
			dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
			dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			break;

		case Mode::DSSLessEqual:
			//mirror
			//do not write depth message
			dsDesc.DepthEnable = true;
			dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
			dsDesc.StencilEnable = false;

			break;
		case Mode::DSSWriteStencil:
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
		case Mode::DSSDrawWithStencil:

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
		case Mode::DSSNoDoubleBlend:

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
		case Mode::DSSNoDepthTest:

			dsDesc.DepthEnable = false;
			dsDesc.StencilEnable = false;
			break;
		case Mode::DSSNoDepthWrite:

			dsDesc.DepthEnable = true;
			dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
			dsDesc.StencilEnable = false;
			break;
		case Mode::DSSNoDepthTestWithStencil:

			dsDesc.DepthEnable = false;
			dsDesc.StencilEnable = true;
			dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
			dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

			dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
			// 
			dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
			break;
		case Mode::DSSNoDepthWriteWithStencil:
			dsDesc.DepthEnable = true;
			dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
			dsDesc.StencilEnable = false;

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
		default:
			break;
		}

		GFX_THROW_INFO(GetDevice(gfx)->CreateDepthStencilState(&dsDesc, DStencil.GetAddressOf()));

	}

	void Bind::DepthStencil::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->OMSetDepthStencilState(DStencil.Get(), 0xFF);
	}

	std::shared_ptr<DepthStencil> Bind::DepthStencil::Resolve(Graphics& gfx, Mode Dss)
	{
		return Codex::Resolve<DepthStencil>(gfx, Dss);
	}

	std::string Bind::DepthStencil::GenerateUID(Mode Dss)
	{
		using namespace std::string_literals;
		return typeid(DepthStencil).name() + "#"s + std::to_string((int)Dss);
	}

	std::string Bind::DepthStencil::GetUID() const noexcept
	{
		return GenerateUID(this->Dss);
	}

}