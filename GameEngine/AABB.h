#pragma once
#include <DirectXMath.h>
#include "IntersectData.h"

//axis aligned bounding box
class AABB
{
public:
	AABB(const DirectX::XMVECTOR& minExtents, const DirectX::XMVECTOR& maxExtents)
		:
		m_minExtents(minExtents),
		m_maxExtents(maxExtents)
	{}

	IntersectData IntersectAABB(const AABB& other)const;
	
	const DirectX::XMVECTOR& GetMinExtents()const { return m_minExtents; }
	const DirectX::XMVECTOR& GetMaxExtents()const { return m_maxExtents; }
private:
	const DirectX::XMVECTOR m_minExtents = {};
	const DirectX::XMVECTOR m_maxExtents = {};
};
