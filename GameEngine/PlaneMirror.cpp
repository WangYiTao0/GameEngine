#include "PlaneMirror.h"
#include "Plane.h"
#include "BindableCommon.h"

PlaneMirror::PlaneMirror(Graphics& gfx, float size)
{

}

DirectX::XMMATRIX PlaneMirror::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(rollPitchYaw.x, rollPitchYaw.y, rollPitchYaw.z) *
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z) *
		DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
}
