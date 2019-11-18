#pragma once
#include <DirectXMath.h>

class Hill
{
public:
	float GetHillsHeight(float x, float z)const
	{
		return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z));
	}

	DirectX::XMFLOAT3 GetHillsNormal(float x, float z)const
	{
		// n = (-df/dx, 1, -df/dz)
		DirectX::XMFLOAT3 n(
			-0.03f * z * cosf(0.1f * x) - 0.3f * cosf(0.1f * z),
			1.0f,
			-0.3f * sinf(0.1f * x) + 0.03f * x * sinf(0.1f * z));

		DirectX::XMVECTOR unitNormal = DirectX::XMVector3Normalize(XMLoadFloat3(&n));
		DirectX::XMStoreFloat3(&n, unitNormal);

		return n;
	}
};