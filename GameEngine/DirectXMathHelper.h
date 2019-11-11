#pragma once
#include <DirectXMath.h>

namespace DirectX
{
	bool XM_CALLCONV operator == (XMFLOAT4X4& lhs, XMFLOAT4X4& rhs);

	bool XM_CALLCONV operator == (XMMATRIX& lhs, XMMATRIX& rhs);
}

DirectX::XMFLOAT3 ExtractEulerAngles(const DirectX::XMFLOAT4X4& matrix);

DirectX::XMFLOAT3 ExtractTranslation(const DirectX::XMFLOAT4X4& matrix);