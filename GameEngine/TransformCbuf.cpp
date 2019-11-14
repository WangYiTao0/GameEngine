#include "TransformCbuf.h"
namespace Bind
{
	TransformCbuf::TransformCbuf(Graphics& gfx,
		const Drawable& parent, UINT slot)
		:
		parent(parent)
	{
		if (!pVcbuf)
		{
			pVcbuf = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
		}
	}

	void TransformCbuf::Bind(Graphics& gfx) noexcept
	{

		UpdateBindImpl(gfx, GetTransforms(gfx));
	}

	void TransformCbuf::UpdateBindImpl(Graphics& gfx, const Transforms& tf) noexcept
	{	
		pVcbuf->Update(gfx, tf);
		pVcbuf->Bind(gfx);
	}

	TransformCbuf::Transforms TransformCbuf::GetTransforms(Graphics& gfx) noexcept
	{
		const auto modelView = parent.GetTransformXM() * gfx.GetCameraViewMatrix();
		const auto modelViewProj = modelView * gfx.GetProjection();

		//DirectX  are row matrix need transpose to column transpose for HLSL
		return{
			DirectX::XMMatrixTranspose(modelView),
			DirectX::XMMatrixTranspose(modelViewProj)
		};
	}

	std::unique_ptr<VertexConstantBuffer<TransformCbuf::Transforms>>TransformCbuf::pVcbuf;
}