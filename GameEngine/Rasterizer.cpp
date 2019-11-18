#include "Rasterizer.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"

namespace Bind
{
	Rasterizer::Rasterizer(Graphics& gfx, bool twoSided,bool solid)
		:
		twoSided(twoSided),
		solid(solid)
	{
		INFOMAN(gfx);

		D3D11_RASTERIZER_DESC rasterDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
		rasterDesc.CullMode = twoSided ? D3D11_CULL_NONE : D3D11_CULL_BACK;
		rasterDesc.FillMode = solid ? D3D11_FILL_SOLID : D3D11_FILL_WIREFRAME;
		GFX_THROW_INFO(GetDevice(gfx)->CreateRasterizerState(&rasterDesc, &pRasterizer));
	}

	void Rasterizer::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->RSSetState(pRasterizer.Get());
	}

	std::shared_ptr<Rasterizer> Rasterizer::Resolve(Graphics& gfx, bool twoSided, bool solid)
	{
		return Codex::Resolve<Rasterizer>(gfx, twoSided, solid);
	}
	std::string Rasterizer::GenerateUID(bool twoSided, bool solid)
	{
		using namespace std::string_literals;
		return typeid(Rasterizer).name() + "#"s + (twoSided ? "2s" : "1s")
			+ "#"s + (solid ? "so" : "wi");
	}
	std::string Rasterizer::GetUID() const noexcept
	{
		return GenerateUID(twoSided, solid);
	}
}