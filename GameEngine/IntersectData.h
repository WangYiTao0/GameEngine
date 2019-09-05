#pragma once
class IntersectData
{
public:
	IntersectData(const bool doesIntersect,const float distance)
		:
	m_doesIntersect(doesIntersect),
		m_distance(distance)
	{}

	bool GetDoesIntersect() const { return m_doesIntersect; }
	float GetDistance()const { return m_distance; }
private:
	bool m_doesIntersect;
	float m_distance;
};