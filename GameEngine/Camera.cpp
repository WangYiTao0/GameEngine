#include "Camera.h"
#include "imgui/imgui.h"
#include "MathHelper.h"
#include <DirectXMath.h>
using namespace  DirectX;

std::shared_ptr<Camera3D> Camera3D::m_pCamera = nullptr;

Camera3D::Camera3D()

{
	Reset();
}

void Camera3D::Set3DProj(float fov, float aspec, float nearZ, float farZ)
{
	m_NearZ = nearZ;
	m_FarZ = farZ;
	proj = DirectX::XMMatrixPerspectiveFovLH(fov,
		aspec,
		m_NearZ, m_FarZ);
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

void Camera3D::RenderReflection(float height)
{
	DirectX::XMFLOAT3 up, position, lookAt;
	float radians;

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	// For planar reflection invert the Y position of the camera.
	position.x = pos.x;
	position.y = -pos.y + (height * 2.0f);
	position.z = pos.z;

	// Calculate the rotation in radians.
	radians = rollPitchYaw.y * MathHelper::oneRad;

	// Setup where the camera is looking.
	lookAt.x = sinf(radians) + pos.x;
	lookAt.y = position.y;
	lookAt.z = cosf(radians) + pos.z;

	// Create the view matrix from the three vectors.
	m_reflectionViewMatrix =  DirectX::XMMatrixLookAtLH(
		XMLoadFloat3(&position), XMLoadFloat3(&lookAt), XMLoadFloat3(&up));
}

DirectX::XMMATRIX Camera3D::GetReflectionViewMatrix()
{
	return m_reflectionViewMatrix;
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

std::shared_ptr<Camera3D> Camera3D::Get()
{
	if (nullptr == m_pCamera)
	{
		m_pCamera = std::shared_ptr<Camera3D>(new Camera3D());
	}
	return m_pCamera;
}

Camera2D::Camera2D()
{
	pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rollPitchYaw = XMFLOAT3(0.0f, 0.0f, 0.0f);
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
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(rollPitchYaw.x, rollPitchYaw.y, rollPitchYaw.z);
	XMMATRIX worldMatrix = camRotationMatrix * translationOffsetMatrix;

	return worldMatrix;
}



