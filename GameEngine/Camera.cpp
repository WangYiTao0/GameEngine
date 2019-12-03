#include "Camera.h"
#include "imgui/imgui.h"
#include "MathHelper.h"
#include <DirectXMath.h>
using namespace  DirectX;

Camera3D::Camera3D(Graphics& gfx)
	:
	gfx(gfx)
{
	Reset();
}

void Camera3D::Set3DProj(float fov, float aspec, float nearZ, float farZ)
{
	proj = DirectX::XMMatrixPerspectiveFovLH(fov,
		aspec,
		nearZ, farZ);
}

DirectX::XMMATRIX Camera3D::GetProj()
{
	return proj;
}

DirectX::XMMATRIX Camera3D::GetViewMatrix() const noexcept
{
	const XMVECTOR forwardBaseVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	// apply the camera rotations to a base vector
	const auto lookVector = XMVector3Transform(forwardBaseVector,
		XMMatrixRotationRollPitchYaw(rollPitchYaw.x, rollPitchYaw.y, 0.0f)
	);
	// generate camera transform (applied to all objects to arrange them relative
	// to camera position/orientation in world) from cam position and direction
	// camera "top" always faces towards +Y (cannot do a barrel roll)
	const auto camPosition = XMLoadFloat3(&pos);
	const auto camTarget = camPosition + lookVector;

	//view matrix
	//Builds a view matrix for a left-handed coordinate system using a camera position, an up direction, and a focal point.
	return XMMatrixLookAtLH(camPosition, camTarget, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
}

void Camera3D::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Camera"))
	{
		if (ImGui::Begin("Camera"))
		{
			ImGui::Text("Position");
			ImGui::SliderFloat("X", &pos.x, -80.0f, 80.0f, "%.1f");
			ImGui::SliderFloat("Y", &pos.y, -80.0f, 80.0f, "%.1f");
			ImGui::SliderFloat("Z", &pos.z, -80.0f, 80.0f, "%.1f");
			ImGui::Text("Orientation");
			ImGui::SliderAngle("Pitch X", &rollPitchYaw.x, 0.995f * -90.0f, 0.995f * 90.0f);
			ImGui::SliderAngle("Yaw   Y", &rollPitchYaw.y, -180.0f, 180.0f);
			if (ImGui::Button("Reset"))
			{
				Reset();
			}
		}
		ImGui::End();
	}
	ImGui::End();
}

void Camera3D::Reset() noexcept
{
	pos = { 0.0f,10.0f,-10.0f };
	rollPitchYaw.x = DirectX::XMConvertToRadians(10.0f);
	rollPitchYaw.y = 0.0f;
}

void Camera3D::Rotate(float dx, float dy) noexcept
{
	rollPitchYaw.y = MathHelper::wrap_angle(rollPitchYaw.y + dx * rotationSpeed);
	rollPitchYaw.x = std::clamp(rollPitchYaw.x + dy * rotationSpeed, 0.995f * -MathHelper::PI / 2.0f, 0.995f * MathHelper::PI / 2.0f);
}

void Camera3D::Translate(DirectX::XMFLOAT3 translation) noexcept
{
	XMStoreFloat3(&translation, 
		XMVector3Transform(XMLoadFloat3(&translation),
			XMMatrixRotationRollPitchYaw(rollPitchYaw.x, rollPitchYaw.y, 0.0f) *
		XMMatrixScaling(travelSpeed, travelSpeed, travelSpeed)
	));
	pos = {
		pos.x + translation.x,
		pos.y + translation.y,
		pos.z + translation.z
	};
}

Camera2D::Camera2D()
{
	pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void Camera2D::SetOrtho(float width, float height, float nearZ, float farZ)
{
	orthoMatrix = XMMatrixOrthographicOffCenterLH(0.0f, width, height, 0.0f, nearZ, farZ);
}

const DirectX::XMMATRIX& Camera2D::GetOrthoMatrix() const
{
	return orthoMatrix;
}

DirectX::XMMATRIX Camera2D::GetWorldMatrix()
{

	XMMATRIX translationOffsetMatrix = XMMatrixTranslation(-pos.x, -pos.y, 0.0f); //z component irrelevant for 2d camera
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
	XMMATRIX worldMatrix = camRotationMatrix * translationOffsetMatrix;

	return worldMatrix;
}
