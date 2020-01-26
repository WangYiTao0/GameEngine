#pragma once
#include "Bindable.h"

namespace Bind
{
	class DepthStencil : public Bindable
	{
	public:
		enum class Mode
		{
			DSSDefault,
			DSSLessEqual,
			DSSWrite,
			DSSMask,
			DSSWriteStencil,
			DSSDrawWithStencil,
			DSSNoDoubleBlend,
			DSSNoDepthTest,
			DSSNoDepthWrite,
			DSSNoDepthTestWithStencil,
			DSSNoDepthWriteWithStencil,
		};

	public:
		DepthStencil(Graphics& gfx, Mode Dss);
		void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<DepthStencil> Resolve(Graphics& gfx, Mode Dss);
		static std::string GenerateUID(Mode Dss);
		std::string GetUID() const noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DStencil;
		Mode Dss;

	};
}
