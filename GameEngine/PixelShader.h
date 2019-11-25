#pragma once
#include "Bindable.h"
namespace Bind
{
	class PixelShader : public Bindable
	{
	public:
		PixelShader(Graphics& gfx, const std::string& csoPath, const std::string& hlslPath);
		void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<PixelShader> Resolve(Graphics& gfx, const std::string& csoPath, const std::string& hlslPath);
		static std::string GenerateUID(const std::string& csoPath, const std::string& hlslPath);
		std::string GetUID() const noexcept override;
	protected:
		std::string csoPath;
		std::string hlslPath;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
		Microsoft::WRL::ComPtr<ID3DBlob> pBlobOut;
	};
}