#pragma once
#include "Bindable.h"

namespace Bind
{
	class DepthStencil_Mask : public Bindable
	{
	public:
		DepthStencil_Mask(Graphics& gfx);
		void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<DepthStencil_Mask> Resolve(Graphics& gfx);
		static std::string GenerateUID();
		std::string GetUID() const noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDS_Mask;
	};
}
