#include "BoundingSphere.h"

IntersectData BoundingSphere::IntersectBoundingSphere(const BoundingSphere& other)
{
	namespace dx = DirectX;

	float radiusDistance = m_radius + other.m_radius;

	auto center = dx::XMLoadFloat3(&m_center);
	auto Ocenter = dx::XMLoadFloat3(&other.GetCenter());
	dx::XMFLOAT3 centerDistance;
	dx::XMStoreFloat3(&centerDistance, dx::XMVector3Length(dx::XMVectorSubtract (center , Ocenter)));

	return IntersectData(centerDistance.x < radiusDistance, centerDistance.x - radiusDistance);
}

