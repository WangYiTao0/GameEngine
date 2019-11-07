#pragma once
#include "Bindable.h"

namespace Bind
{
	class BlendState : public Bindable
	{
	public:
		BlendState(Graphics& gfx);
		void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<BlendState> Resolve(Graphics& gfx);
		static std::string GenerateUID();
		std::string GetUID() const noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11BlendState> pBlendState;
	};
}