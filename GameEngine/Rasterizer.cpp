#include "Rasterizer.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"

namespace Bind
{
	Rasterizer::Rasterizer(Graphics& gfx, Mode Rs)
		:
		Rs(Rs)
	{
		INFOMAN(gfx);

		D3D11_RASTERIZER_DESC rasterDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});

		switch (Rs)
		{
		case Bind::Rasterizer::Mode::RSWireframe:
			rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
			rasterDesc.CullMode = D3D11_CULL_NONE;
			rasterDesc.FrontCounterClockwise = false;
			rasterDesc.DepthClipEnable = true;
			break;
		case Bind::Rasterizer::Mode::RSNoCull:
			rasterDesc.FillMode = D3D11_FILL_SOLID;
			rasterDesc.CullMode = D3D11_CULL_NONE;
			rasterDesc.FrontCounterClockwise = false;
			rasterDesc.DepthClipEnable = true;
			break;
		case Bind::Rasterizer::Mode::RSCullClockWise:
			rasterDesc.FillMode = D3D11_FILL_SOLID;
			rasterDesc.CullMode = D3D11_CULL_BACK;
			rasterDesc.FrontCounterClockwise = true;
			rasterDesc.DepthClipEnable = true;
			break;
		case Mode::RSCullBack:
			rasterDesc.FillMode = D3D11_FILL_SOLID;
			rasterDesc.CullMode = D3D11_CULL_BACK;
			rasterDesc.FrontCounterClockwise = false;
			rasterDesc.DepthClipEnable = true;
			break;
		case Mode::RSCullFront:
			rasterDesc.FillMode = D3D11_FILL_SOLID;
			rasterDesc.CullMode = D3D11_CULL_FRONT;
			rasterDesc.FrontCounterClockwise = false;
			rasterDesc.DepthClipEnable = true;
			break;
		case Mode::RSDepthDisable:
			rasterDesc.FillMode = D3D11_FILL_SOLID;
			rasterDesc.CullMode = D3D11_CULL_BACK;
			rasterDesc.FrontCounterClockwise = false;
			rasterDesc.DepthClipEnable = false;
			break;
		default:
			break;
		}

		//rasterDesc.CullMode = twoSided ? D3D11_CULL_NONE : D3D11_CULL_BACK;
		//rasterDesc.FillMode = solid ? D3D11_FILL_SOLID : D3D11_FILL_WIREFRAME;
		GFX_THROW_INFO(GetDevice(gfx)->CreateRasterizerState(&rasterDesc, &pRasterizer));
	}

	void Rasterizer::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->RSSetState(pRasterizer.Get());
	}

	std::shared_ptr<Rasterizer> Rasterizer::Resolve(Graphics& gfx, Mode Rs)
	{
		return Codex::Resolve<Rasterizer>(gfx, Rs);
	}
	std::string Rasterizer::GenerateUID(Mode Rs)
	{
		using namespace std::string_literals;
		return typeid(Rasterizer).name() + "#"s + std::to_string(int(Rs));
	}
	std::string Rasterizer::GetUID() const noexcept
	{
		return GenerateUID(Rs);
	}
}