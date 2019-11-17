#pragma once
#include "Bindable.h"
namespace Bind
{
	class Sampler : public Bindable
	{
	public:
		Sampler(Graphics& gfx, UINT slot = 0);
		void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<Sampler> Resolve(Graphics& gfx);
		static std::string GenerateUID(UINT slot = 0);
		std::string GetUID() const noexcept override;
	private:
		unsigned int slot;
	protected:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	};
}