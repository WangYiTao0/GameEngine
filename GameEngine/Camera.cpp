#include "Camera.h"
#include "imgui/imgui.h"
#include "ChiliMath.h"
namespace dx = DirectX;

//Camera::Camera() 
//{
//	Reset();
//}

Camera::Camera(Graphics& gfx)
	:
	gfx(gfx)
{
	Reset();
}

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	using namespace dx;

	const dx::XMVECTOR forwardBaseVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	// apply the camera rotations to a base vector
	const auto lookVector = XMVector3Transform(forwardBaseVector,
		dx::XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f)
	);
	// generate camera transform (applied to all objects to arrange them relative
	// to camera position/orientation in world) from cam position and direction
	// camera "top" always faces towards +Y (cannot do a barrel roll)
	const auto camPosition = XMLoadFloat3(&pos);
	const auto camTarget = camPosition + lookVector;
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
	yaw = wrap_angle(yaw + dx * rotationSpeed);
	pitch = std::clamp(pitch + dy * rotationSpeed, 0.995f * -PI / 2.0f, 0.995f * PI / 2.0f);
}

void Camera::Translate(DirectX::XMFLOAT3 translation) noexcept
{
	dx::XMStoreFloat3(&translation, dx::XMVector3Transform(
		dx::XMLoadFloat3(&translation),
		dx::XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f) *
		dx::XMMatrixScaling(travelSpeed, travelSpeed, travelSpeed)
	));
	pos = {
		pos.x + translation.x,
		pos.y + translation.y,
		pos.z + translation.z
	};
}


bool Camera::GetVisibility(DirectX::XMFLOAT3 Position)
{
	DirectX::XMVECTOR worldPos, viewPos, ProjPos;
	DirectX::XMFLOAT3 ProjPosF;
	worldPos = DirectX::XMLoadFloat3(&Position);
	viewPos = DirectX::XMVector3TransformCoord(worldPos, GetMatrix());
	ProjPos = DirectX::XMVector3TransformCoord(viewPos, gfx.GetProjection());
	DirectX::XMStoreFloat3(&ProjPosF, ProjPos);

	if (-1.0f < ProjPosF.x && ProjPosF.x < 1.0f &&
		-1.0f < ProjPosF.y && ProjPosF.y < 1.0f &&
		0.0f < ProjPosF.z && ProjPosF.z < 1.0f)
	{
		return true;
	}

	return false;
}


/*
void Ball::Draw()
{
	CCamera* camera;
	camera = CManager::GetSceme()->GetGameObject<CCamera>();
	if(camera->GetVisibility(m_Postion)==false)
	{
		return;
	}
}
*/