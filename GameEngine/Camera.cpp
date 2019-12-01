#include "Camera.h"
#include "imgui/imgui.h"
#include "MathHelper.h"
#include <DirectXMath.h>
using namespace  DirectX;

Camera::Camera(Graphics& gfx)
	:
	gfx(gfx)
{
	Reset();
}

DirectX::XMMATRIX Camera::GetViewMatrix() const noexcept
{


	const XMVECTOR forwardBaseVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	// apply the camera rotations to a base vector
	const auto lookVector = XMVector3Transform(forwardBaseVector,
		XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f)
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

void Camera::SpawnControlWindow() noexcept
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
			ImGui::SliderAngle("Pitch X", &pitch, 0.995f * -90.0f, 0.995f * 90.0f);
			ImGui::SliderAngle("Yaw   Y", &yaw, -180.0f, 180.0f);
			if (ImGui::Button("Reset"))
			{
				Reset();
			}
		}
		ImGui::End();
	}
	ImGui::End();
}

void Camera::Reset() noexcept
{
	pos = { 0.0f,10.0f,-10.0f };
	pitch = DirectX::XMConvertToRadians(10.0f);
	yaw = 0.0f;

}

void Camera::Rotate(float dx, float dy) noexcept
{
	yaw = MathHelper::wrap_angle(yaw + dx * rotationSpeed);
	pitch = std::clamp(pitch + dy * rotationSpeed, 0.995f * -MathHelper::PI / 2.0f, 0.995f * MathHelper::PI / 2.0f);
}

void Camera::Translate(DirectX::XMFLOAT3 translation) noexcept
{
	XMStoreFloat3(&translation, 
		XMVector3Transform(XMLoadFloat3(&translation),XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f) *
		XMMatrixScaling(travelSpeed, travelSpeed, travelSpeed)
	));
	pos = {
		pos.x + translation.x,
		pos.y + translation.y,
		pos.z + translation.z
	};
}


DirectX::XMFLOAT3 Camera::GetPos() const noexcept
{
	return pos;
}

