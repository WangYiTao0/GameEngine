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

DirectX::XMFLOAT3 DXMH::ExtractEulerAngles(const DirectX::XMFLOAT4X4& mat)
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

DirectX::XMFLOAT3 DXMH::ExtractTranslation(const DirectX::XMFLOAT4X4& matrix)
{
	return { matrix._41,matrix._42,matrix._43 };
}



float DXMH::Distance(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	XMVECTOR vector1 = XMLoadFloat3(&v1);
	XMVECTOR vector2 = XMLoadFloat3(&v2);
	XMVECTOR vectorSub = XMVectorSubtract(vector1, vector2);
	XMVECTOR length = XMVector3Length(vectorSub);

	float Distance = 0.0f;
	XMStoreFloat(&Distance, length);
	return Distance;
}
float DXMH::DistanceSqaured(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	XMVECTOR vector1 = XMLoadFloat3(&v1);
	XMVECTOR vector2 = XMLoadFloat3(&v2);
	XMVECTOR vectorSub = XMVectorSubtract(vector1, vector2);
	XMVECTOR length = XMVector3LengthSq(vectorSub);

	float Distance = 0.0f;
	XMStoreFloat(&Distance, length);
	return Distance;
}
float DXMH::DistanceEstimated(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	XMVECTOR vector1 = XMLoadFloat3(&v1);
	XMVECTOR vector2 = XMLoadFloat3(&v2);
	XMVECTOR vectorSub = XMVectorSubtract(vector1, vector2);
	XMVECTOR length = XMVector3LengthEst(vectorSub);

	float Distance = 0.0f;
	XMStoreFloat(&Distance, length);
	return Distance;
}
XMFLOAT3 DXMH::getVectorHalfWayPoint(const XMFLOAT3& a, const XMFLOAT3& b)
{
	return XMFLOAT3((a.x + b.x) * 0.5f, (a.y + b.y) * 0.5f, (a.z + b.z) * 0.5f);
}

bool DXMH::Collision(const XMFLOAT2& hitBox1Pos, const XMFLOAT2& hitBox1Siz, const XMFLOAT2& hitBox2Pos, const XMFLOAT2& hitBox2Siz)
{
	if (hitBox1Pos.x + hitBox1Siz.x < hitBox2Pos.x)
		return false;
	else if (hitBox1Pos.x > hitBox2Pos.x + hitBox2Siz.x)
		return false;
	else if (hitBox1Pos.y - hitBox1Siz.y > hitBox2Pos.y)
		return false;
	else if (hitBox1Pos.y < hitBox2Pos.y - hitBox2Siz.y)
		return false;
	else
		return true;
}



float DXMH::Lerp(const float& value1, const float& value2, const float& amount)
{
	return value1 + (value2 - value1) * amount;
}
XMFLOAT2 DXMH::Lerp(const XMFLOAT2& a, const XMFLOAT2& b, const float& i)
{
	return XMFLOAT2(Lerp(a.x, b.x, i), Lerp(a.y, b.y, i));
}
XMFLOAT3 DXMH::Lerp(const XMFLOAT3& a, const XMFLOAT3& b, const float& i)
{
	return XMFLOAT3(Lerp(a.x, b.x, i), Lerp(a.y, b.y, i), Lerp(a.z, b.z, i));
}
XMFLOAT4 DXMH::Lerp(const XMFLOAT4& a, const XMFLOAT4& b, const float& i)
{
	return XMFLOAT4(Lerp(a.x, b.x, i), Lerp(a.y, b.y, i), Lerp(a.z, b.z, i), Lerp(a.w, b.w, i));
}
XMFLOAT3 DXMH::Max(const XMFLOAT3& a, const XMFLOAT3& b) {
	return XMFLOAT3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}
XMFLOAT3 DXMH::Min(const XMFLOAT3& a, const XMFLOAT3& b) {
	return XMFLOAT3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
}
float DXMH::Clamp(float val, float min, float max)
{
	if (val < min) return min;
	else if (val > max) return max;
	return val;
}

XMFLOAT3 DXMH::getCubicHermiteSplinePos(const XMFLOAT3& startPos, const XMFLOAT3& endPos
	, const XMFLOAT3& startTangent, const XMFLOAT3& endTangent
	, const float& atInterval) {
	float x, y, z, t; float r1 = 1.0f, r4 = 1.0f;
	t = atInterval;
	x = (2 * t * t * t - 3 * t * t + 1) * startPos.x + (-2 * t * t * t + 3 * t * t) * endPos.x + (t * t * t - 2 * t * t + t) * startTangent.x + (t * t * t - t * t) * endTangent.x;
	y = (2 * t * t * t - 3 * t * t + 1) * startPos.y + (-2 * t * t * t + 3 * t * t) * endPos.y + (t * t * t - 2 * t * t + 1) * startTangent.y + (t * t * t - t * t) * endTangent.y;
	z = (2 * t * t * t - 3 * t * t + 1) * startPos.z + (-2 * t * t * t + 3 * t * t) * endPos.z + (t * t * t - 2 * t * t + 1) * startTangent.z + (t * t * t - t * t) * endTangent.z;
	return XMFLOAT3(x, y, z);
}
XMFLOAT3 DXMH::getQuadraticBezierPos(const XMFLOAT3& a, const XMFLOAT3& b, const XMFLOAT3& c, const float& t) {
	float param0, param1, param2;
	param0 = pow(1 - t, 2);
	param1 = 2 * (1 - t) * t;
	param2 = pow(t, 2);
	float x = param0 * a.x + param1 * b.x + param2 * c.x;
	float y = param0 * a.y + param1 * b.y + param2 * c.y;
	float z = param0 * a.z + param1 * b.z + param2 * c.z;
	return XMFLOAT3(x, y, z);
}
XMFLOAT3 DXMH::getQuadraticBezierPos(const XMFLOAT4& a, const XMFLOAT4& b, const XMFLOAT4& c, const float& t) {
	return getQuadraticBezierPos(XMFLOAT3(a.x, a.y, a.z), XMFLOAT3(b.x, b.y, b.z), XMFLOAT3(c.x, c.y, c.z), t);
}
