#pragma once
#include "Bindable.h"

namespace Bind
{
	class DepthStencil : public Bindable
	{
	public:
		enum class DepthStencilState
		{
			DSSLessEqual,
			DSSWriteStencil,
			DSSDrawWithStencil,
			DSSNoDoubleBlend,
			DSSNoDepthTest,
			DSSNoDepthWrite,
			DSSNoDepthTestWithStencil,
			DSSNoDepthWriteWithStencil,
		};

	public:
		DepthStencil(Graphics& gfx, DepthStencilState Dss);
		void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<DepthStencil> Resolve(Graphics& gfx, DepthStencilState Dss);
		static std::string GenerateUID(DepthStencilState Dss);
		std::string GetUID() const noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DStencil;
		DepthStencilState Dss;

	};
}
