#pragma once
#include "Bindable.h"
namespace Bind
{
	class Sampler : public Bindable
	{
	public:
		enum class SamplerState
		{
			SSLinearWrap,
			SSLinearClamp,
			SSAnistropicWrap,
		};

	public:
		Sampler(Graphics& gfx, UINT slot, SamplerState samplerState);
		void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<Sampler> Resolve(Graphics& gfx, UINT slot, SamplerState samplerState);
		static std::string GenerateUID(UINT slot, SamplerState samplerState);
		std::string GetUID() const noexcept override;
	private:
		unsigned int slot;
	protected:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
		SamplerState samplerState;
	};
}