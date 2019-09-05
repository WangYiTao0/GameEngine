#include "BoundingSphere.h"

IntersectData BoundingSphere::IntersectBoundingSphere(const BoundingSphere& other)
{
	using namespace DirectX;

	float radiusDistance = m_radius + other.m_radius;

	auto center = DirectX::XMLoadFloat3(&m_center);
	auto Ocenter = DirectX::XMLoadFloat3(&other.GetCenter());
	XMFLOAT3 centerDistance;
	XMStoreFloat3(&centerDistance, XMVector3Length(center - Ocenter));

	return IntersectData(centerDistance.x < radiusDistance, centerDistance.x - radiusDistance);
}

