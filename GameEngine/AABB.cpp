#include "AABB.h"

AABB::AABB(const MH::Vec3* points, size_t num_points)
{
	m_minExtents = { FLT_MAX, FLT_MAX, FLT_MAX };
	m_maxExtents = { FLT_MIN, FLT_MIN, FLT_MIN };

	for (size_t i = 0; i < num_points; i++)
	{
		const MH::Vec3& point = points[i];

		if (point.x < m_minExtents.x) m_minExtents.x = point.x;
		if (point.y < m_minExtents.y) m_minExtents.y = point.y;
		if (point.z < m_minExtents.z) m_minExtents.z = point.z;

		if (point.x > m_maxExtents.x) m_maxExtents.x = point.x;
		if (point.y > m_maxExtents.y) m_maxExtents.y = point.y;
		if (point.z > m_maxExtents.z) m_maxExtents.z = point.z;
	}
}

AABB AABB::Transform(DirectX::XMMATRIX transform) const
{
	using namespace MH;
	Vec3 transformed_m_minExtents = DirectX::XMVector3Transform(m_minExtents, transform);
	Vec3 transformed_m_maxExtents = DirectX::XMVector3Transform(m_maxExtents, transform);

	AABB aabb;
	aabb.m_minExtents = { FLT_MAX, FLT_MAX, FLT_MAX };
	aabb.m_maxExtents = { FLT_MIN, FLT_MIN, FLT_MIN };
	if (transformed_m_minExtents.x < aabb.m_minExtents.x) aabb.m_minExtents.x = transformed_m_minExtents.x;
	if (transformed_m_minExtents.y < aabb.m_minExtents.y) aabb.m_minExtents.y = transformed_m_minExtents.y;
	if (transformed_m_minExtents.z < aabb.m_minExtents.z) aabb.m_minExtents.z = transformed_m_minExtents.z;
	if (transformed_m_maxExtents.x < aabb.m_minExtents.x) aabb.m_minExtents.x = transformed_m_maxExtents.x;
	if (transformed_m_maxExtents.y < aabb.m_minExtents.y) aabb.m_minExtents.y = transformed_m_maxExtents.y;
	if (transformed_m_maxExtents.z < aabb.m_minExtents.z) aabb.m_minExtents.z = transformed_m_maxExtents.z;
	if (transformed_m_minExtents.x > aabb.m_maxExtents.x) aabb.m_maxExtents.x = transformed_m_minExtents.x;
	if (transformed_m_minExtents.y > aabb.m_maxExtents.y) aabb.m_maxExtents.y = transformed_m_minExtents.y;
	if (transformed_m_minExtents.z > aabb.m_maxExtents.z) aabb.m_maxExtents.z = transformed_m_minExtents.z;
	if (transformed_m_maxExtents.x > aabb.m_maxExtents.x) aabb.m_maxExtents.x = transformed_m_maxExtents.x;
	if (transformed_m_maxExtents.y > aabb.m_maxExtents.y) aabb.m_maxExtents.y = transformed_m_maxExtents.y;
	if (transformed_m_maxExtents.z > aabb.m_maxExtents.z) aabb.m_maxExtents.z = transformed_m_maxExtents.z;

	return aabb;
}

void AABB::GetPoints(MH::Vec3 out[8]) const
{
	out[0] = { m_minExtents.x, m_minExtents.y, m_minExtents.z };
	out[1] = { m_minExtents.x, m_minExtents.y, m_maxExtents.z };
	out[2] = { m_maxExtents.x, m_minExtents.y, m_maxExtents.z };
	out[3] = { m_maxExtents.x, m_minExtents.y, m_minExtents.z };
	out[4] = { m_maxExtents.x, m_maxExtents.y, m_maxExtents.z };
	out[5] = { m_minExtents.x, m_maxExtents.y, m_maxExtents.z };
	out[6] = { m_minExtents.x, m_maxExtents.y, m_minExtents.z };
	out[7] = { m_maxExtents.x, m_maxExtents.y, m_minExtents.z };
}

IntersectData AABB::IntersectAABB(const AABB& other) const
{
	namespace dx = DirectX;
	dx::XMVECTOR distances1 = dx::XMVectorSubtract(other.GetMinExtents() , m_maxExtents);
	dx::XMVECTOR distances2 = dx::XMVectorSubtract(m_minExtents, other.GetMaxExtents());
	//dx::XMVECTOR distances = distances.Max(distances2);
	float maxDistance = 0;
	//float maxDistance = distances.Max();

	return IntersectData(maxDistance < 0, maxDistance);
	
}
