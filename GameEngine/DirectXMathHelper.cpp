#include "DirectXMathHelper.h"

namespace DirectX
{
	bool XM_CALLCONV operator == (XMFLOAT4X4& lhs, XMFLOAT4X4& rhs)
	{
		if (lhs._11 == rhs._11 || lhs._12 == rhs._12 || lhs._13, rhs._13 || lhs._14 == rhs._14
			|| lhs._21 == rhs._21 || lhs._22 == rhs._22 || lhs._23, rhs._23 || lhs._24 == rhs._24
			|| lhs._31 == rhs._31 || lhs._32 == rhs._32 || lhs._33, rhs._33 || lhs._34 == rhs._34
			|| lhs._41 == rhs._41 || lhs._42 == rhs._42 || lhs._43, rhs._43 || lhs._44 == rhs._44)
		{
			return true;
		}
		return false;
	}

	bool XM_CALLCONV operator == (XMMATRIX& lhs, XMMATRIX& rhs)
	{
		XMFLOAT4X4 flhs = {};
		XMFLOAT4X4 frhs = {};
		XMStoreFloat4x4(&flhs, lhs);
		XMStoreFloat4x4(&frhs, rhs);

		if (flhs == frhs)
		{
			return true;
		}

		return false;
	}
}

DirectX::XMFLOAT3 ExtractEulerAngles(const DirectX::XMFLOAT4X4& mat)
{
	DirectX::XMFLOAT3 euler;

	euler.x = asinf(-mat._32);                  // Pitch
	if (cosf(euler.x) > 0.0001)                // Not at poles
	{
		euler.y = atan2f(mat._31, mat._33);      // Yaw
		euler.z = atan2f(mat._12, mat._22);      // Roll
	}
	else
	{
		euler.y = 0.0f;                           // Yaw
		euler.z = atan2f(-mat._21, mat._11);     // Roll
	}

	return euler;
}

DirectX::XMFLOAT3 ExtractTranslation(const DirectX::XMFLOAT4X4& matrix)
{
	return { matrix._41,matrix._42,matrix._43 };
}
