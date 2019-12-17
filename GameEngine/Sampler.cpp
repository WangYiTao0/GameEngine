#include "Sampler.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"
namespace Bind
{
	Sampler::Sampler(Graphics& gfx, UINT slot, SamplerState samplerState)
		:
		slot(slot),
		samplerState(samplerState)
	{
		INFOMAN(gfx);

		D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC{ CD3D11_DEFAULT{} };

		switch (this->samplerState)
		{
		case SamplerState::SSAnistropicWrap:
			samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
			break;
		case SamplerState::SSLinearWrap:
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			break;
		case SamplerState::SSLinearClamp:
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.MipLODBias = 0.0f;
			samplerDesc.MaxAnisotropy = 1;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
			samplerDesc.BorderColor[0] = 0;
			samplerDesc.BorderColor[1] = 0;
			samplerDesc.BorderColor[2] = 0;
			samplerDesc.BorderColor[3] = 0;
			samplerDesc.MinLOD = 0;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		default:
			break;
		}

		GFX_THROW_INFO(GetDevice(gfx)->CreateSamplerState(&samplerDesc, &pSampler));
	}

	void Sampler::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->PSSetSamplers(slot, 1, pSampler.GetAddressOf());
	}
	std::shared_ptr<Sampler> Sampler::Resolve(Graphics& gfx,UINT slot, SamplerState samplerState)
	{
		return Codex::Resolve<Sampler>(gfx, slot, samplerState);
	}
	std::string Sampler::GenerateUID(UINT slot,SamplerState samplerState)
	{
		using namespace std::string_literals;
		return typeid(Sampler).name() + "#"s + std::to_string(slot) + "#"s + std::to_string(int(samplerState));
	}
	std::string Sampler::GetUID() const noexcept
	{
		return GenerateUID(this->slot,this->samplerState);
	}
}