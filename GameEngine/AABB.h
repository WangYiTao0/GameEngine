#pragma once
#include <DirectXMath.h>
#include "IntersectData.h"
#include "MathHelper.h"


//axis aligned bounding box
class AABB
{
public:
	AABB() = default;
	AABB(const MH::Vec3& minExtents, const MH::Vec3& maxExtents)
		:
		m_minExtents(minExtents),
		m_maxExtents(maxExtents)
	{}

	AABB(const MH::Vec3* points, size_t num_points);

	MH::Vec3 GetCenter() const { return (m_minExtents + m_maxExtents) / 2; }

	AABB Transform(DirectX::XMMATRIX transform) const;
	void GetPoints(MH::Vec3 out[8]) const;

	IntersectData IntersectAABB(const AABB& other)const;
	
	const MH::Vec3& GetMinExtents()const { return m_minExtents; }
	const MH::Vec3& GetMaxExtents()const { return m_maxExtents; }
private:
	MH::Vec3 m_minExtents = {};
	MH::Vec3 m_maxExtents = {};
};
