#pragma once
#include "Bindable.h"
#include <array>

namespace Bind
{
	class Rasterizer : public Bindable
	{
	public:
		enum class Mode
		{
			RSWireframe,
			RSNoCull,
			RSCullClockWise,
			RSCull,
			RSDepthDisable
		};

	public:
		Rasterizer(Graphics& gfx, Mode Rs);
		void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<Rasterizer> Resolve(Graphics& gfx, Mode Rs);
		static std::string GenerateUID(Mode Rs);
		std::string GetUID() const noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> pRasterizer;
		Mode Rs;
	};
}
