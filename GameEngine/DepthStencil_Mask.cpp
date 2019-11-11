#include "DepthStencil_Mask.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"
namespace Bind
{
	DepthStencil_Mask::DepthStencil_Mask(Graphics& gfx)
	{

		CD3D11_DEPTH_STENCIL_DESC dsd = {};

	}

	void Bind::DepthStencil_Mask::Bind(Graphics& gfx) noexcept
	{
	}

	std::shared_ptr<DepthStencil_Mask> Bind::DepthStencil_Mask::Resolve(Graphics& gfx)
	{
		return std::shared_ptr<DepthStencil_Mask>();
	}

	std::string Bind::DepthStencil_Mask::GenerateUID()
	{
		return std::string();
	}

	std::string Bind::DepthStencil_Mask::GetUID() const noexcept
	{
		return std::string();
	}

}