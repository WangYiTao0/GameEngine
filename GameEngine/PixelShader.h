#pragma once
#include "Bindable.h"
namespace Bind
{
	class PixelShader : public Bindable
	{
	public:
		PixelShader(Graphics& gfx, const std::string& shaderName);
		void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<PixelShader> Resolve(Graphics& gfx, const std::string& shaderName);
		static std::string GenerateUID(const std::string& shaderName);
		std::string GetUID() const noexcept override;
	protected:
		std::string m_ShaderName;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
		Microsoft::WRL::ComPtr<ID3DBlob> pBlobOut;
	};
}