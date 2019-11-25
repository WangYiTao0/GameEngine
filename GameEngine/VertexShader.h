#pragma once
#include "Bindable.h"

namespace Bind
{
	class VertexShader : public Bindable
	{
	public:
		VertexShader(Graphics& gfx, const std::string& csoPath, const std::string& hlslPath);
		void Bind(Graphics& gfx)noexcept override;
		ID3DBlob* GetBytecode()const noexcept;
		static std::shared_ptr<VertexShader> Resolve(Graphics& gfx, const std::string& csoPath, const std::string& hlslPath);
		static std::string GenerateUID(const std::string& csoPath, const std::string& hlslPath);
		std::string GetUID() const noexcept override;
	protected:
		std::string csoPath;
		std::string hlslPath;
		Microsoft::WRL::ComPtr<ID3DBlob> pBytecodeBlob;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	};
}