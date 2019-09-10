#include "AABB.h"

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
