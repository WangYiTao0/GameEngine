#include "ViewPoint.h"

void ViewPoint::SetPostion(DirectX::XMFLOAT3 pos)
{
	SetPostion(pos.x, pos.y, pos.z);
}

void ViewPoint::SetLookAt(DirectX::XMFLOAT3 lookat)
{
	SetLookAt(lookat.x, lookat.y, lookat.z);
}

void ViewPoint::SetPostion(float x, float y, float z)
{
	m_Position = DirectX::XMFLOAT3(x, y, z);

}

void ViewPoint::SetLookAt(float x, float y, float z)
{
	m_LookAt = DirectX::XMFLOAT3(x, y, z);
}

void ViewPoint::SetScreen(int width, int height)
{
	m_ScreenWidth = width;
	m_ScreenHeight = height;
}

void ViewPoint::SetProjectionParameters(float FieldOfView, float AspectRatio, float NearPlane, float FarPlane)
{
	m_Fov = FieldOfView;
	m_AspectRatio = AspectRatio;
	m_NearPlane = NearPlane;
	m_FarPlane = FarPlane;
}

void ViewPoint::GenerateViewMatrix()
{
	using namespace DirectX;
	XMVECTOR EyePostion = XMLoadFloat3(&m_Position);
	XMVECTOR LookAt = XMLoadFloat3(&m_LookAt);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//
	m_View = XMMatrixLookToLH(EyePostion, LookAt, Up);

}

void ViewPoint::GenerateProjMatrix()
{
	m_Proj = DirectX::XMMatrixPerspectiveFovLH(m_Fov, m_AspectRatio, m_NearPlane, m_FarPlane);
}

void ViewPoint::GenerateOrthoMatrix()
{
	m_Ortho = DirectX::XMMatrixOrthographicLH(100, 100, m_NearPlane, m_FarPlane);
}

DirectX::XMMATRIX ViewPoint::GetViewMatrix()
{
	return m_View;
}

DirectX::XMMATRIX ViewPoint::GetProjMatrix()
{
	return m_Proj;
}

DirectX::XMMATRIX ViewPoint::GetOrthoMatrix()
{
	return m_Ortho;
}
