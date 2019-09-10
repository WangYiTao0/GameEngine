#pragma once
#include <DirectXMath.h>
class PhysicsObject
{
public:
	PhysicsObject(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& velocity)
		:
		m_position(pos),
		m_velocity(velocity)
	{}
	void Intergrate(float delta);

	const DirectX::XMFLOAT3 GetPosition()const { return m_position; }
	const DirectX::XMFLOAT3 GetVelocity()const { return m_velocity; }

private:
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_velocity;
};