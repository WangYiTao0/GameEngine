#include "PhysicsObject.h"

void PhysicsObject::Intergrate(float delta)
{
	using namespace DirectX;
	auto v_position = XMLoadFloat3(&m_position);
	auto v_velocity = XMLoadFloat3(&m_velocity);
	v_position = XMVectorAdd(v_position, (v_velocity * delta));
	XMStoreFloat3(&m_position, v_position);
}
