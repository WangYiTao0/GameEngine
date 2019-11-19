#pragma once
#include "Bindable.h"
#include <array>

namespace Bind
{
	class Rasterizer : public Bindable
	{
	public:
		enum class RasterizerState
		{
			RSWireframe,
			RSNoCull,
			RSCullClockWise,
			RSCull,
		};

	public:
		Rasterizer(Graphics& gfx, RasterizerState Rs);
		void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<Rasterizer> Resolve(Graphics& gfx, RasterizerState Rs);
		static std::string GenerateUID(RasterizerState Rs);
		std::string GetUID() const noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizer;
		RasterizerState Rs;
	};
}
