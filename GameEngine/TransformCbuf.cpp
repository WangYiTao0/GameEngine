#include "TransformCbuf.h"
#include "MathHelper.h"
#include "Camera.h"
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
		const auto worldInv = MH::Inversse(world);
		const auto view = gfx.GetCameraViewMatrix();
		const auto viewInv = MH::Inversse(view);

		const auto proj = gfx.GetProjection();

		const auto ortho = gfx.GetOrtho();
		const auto world2D = gfx.GetCamera2DWorldMatrix();
		//const auto cameraPos = MH::GetCameraPosFromViewInvMatrix(viewInv);

		const auto cameraPos = GCamera3D->GetPos();

		//DirectX  are row matrix need transpose to column transpose for HLSL
		return{
			DirectX::XMMatrixTranspose(world),
			DirectX::XMMatrixTranspose(worldInv),
			DirectX::XMMatrixTranspose(view),
			DirectX::XMMatrixTranspose(viewInv),
			DirectX::XMMatrixTranspose(proj),
			DirectX::XMMatrixTranspose(ortho),
			DirectX::XMMatrixTranspose(world2D),
			cameraPos,
			0.0f,
		};
	}

	std::unique_ptr<VertexConstantBuffer<TransformCbuf::Transforms>>TransformCbuf::pVcbuf;
}