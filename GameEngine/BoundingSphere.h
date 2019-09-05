#pragma once
#include<DirectXMath.h>
#include "IntersectData.h"

class BoundingSphere
{
public:
	BoundingSphere(const DirectX::XMFLOAT3& center, float radius)
		:
		m_center(center),
		m_radius(radius)
	{}
	IntersectData IntersectBoundingSphere(const BoundingSphere& other);

	const DirectX::XMFLOAT3& GetCenter() const { return m_center; }
	float GetRadius()const { return m_radius; }

private:
	const DirectX::XMFLOAT3 m_center;
	const float m_radius;
};