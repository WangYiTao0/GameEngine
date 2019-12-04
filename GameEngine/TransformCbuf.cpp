#include "TransformCbuf.h"
#include "MathHelper.h"
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
		const auto world = parent.GetTransformXM();
		const auto worldInvTranspose = MathHelper::InverseTranspose(world);
		const auto view = gfx.GetCameraViewMatrix();
		const auto proj = gfx.GetProjection();
		const auto ortho = gfx.GetOrtho();
		const auto world2D = gfx.GetCamera2DWorldMatrix();
		//DirectX  are row matrix need transpose to column transpose for HLSL
		return{
			DirectX::XMMatrixTranspose(world),
			worldInvTranspose,
			DirectX::XMMatrixTranspose(view),
			DirectX::XMMatrixTranspose(proj),
			DirectX::XMMatrixTranspose(ortho),
			DirectX::XMMatrixTranspose(world2D)
		};
	}

	std::unique_ptr<VertexConstantBuffer<TransformCbuf::Transforms>>TransformCbuf::pVcbuf;
}