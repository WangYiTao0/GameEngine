#pragma once
#include "Bindable.h"

namespace Bind
{
	class DepthStencil : public Bindable
	{
	public:
		DepthStencil(Graphics& gfx);
		void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<DepthStencil> Resolve(Graphics& gfx);
		static std::string GenerateUID();
		std::string GetUID() const noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DSSWriteStencil;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DSSDrawWithStencil;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DSSNoDoubleBlend;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DSSNoDepthTest;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DSSNoDepthWrite;
	};
}
