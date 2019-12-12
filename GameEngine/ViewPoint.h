#pragma once
#include<DirectXMath.h>
#include"MathHelper.h"

class ViewPoint
{
public:
	ViewPoint() = default;
	ViewPoint(const ViewPoint&) = default;
	~ViewPoint() = default;
	//Set
	void SetPostion(DirectX::XMFLOAT3 pos);
	void SetLookAt(DirectX::XMFLOAT3 lookat);
	void SetPostion(float x, float y, float z);
	void SetLookAt(float x, float y, float z);
	void SetScreen(int width, int height);
	void SetProjectionParameters(float FieldOfView, float AspectRatio, float NearPlane, float FarPlane);

	//GenerateViewMatrix
	void GenerateViewMatrix();
	//GenerateProjMatrix
	void GenerateProjMatrix();
	//GenerateOrthoMatrix
	void GenerateOrthoMatrix();

	//Get
	DirectX::XMMATRIX GetViewMatrix();
	DirectX::XMMATRIX GetProjMatrix();
	DirectX::XMMATRIX GetOrthoMatrix();
private:
	MH::Vec3 m_Position;
	MH::Vec3 m_LookAt;
	DirectX::XMMATRIX m_View, m_Proj,m_Ortho;

	float m_Fov;
	float m_AspectRatio;
	float m_NearPlane, m_FarPlane;
	float m_ScreenWidth, m_ScreenHeight;
};