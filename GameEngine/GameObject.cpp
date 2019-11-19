#include "GameObject.h"

void GameObject::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
	this->pos = pos;
}

DirectX::XMFLOAT3 GameObject::GetPos() const noexcept
{
	return pos;
}

void GameObject::SetRotation(DirectX::XMFLOAT3 rollPitchYaw) noexcept
{
	this->rollPitchYaw = rollPitchYaw;

}

DirectX::XMFLOAT3 GameObject::GetRotation() const noexcept
{
	return rollPitchYaw;
}

void GameObject::SetScale(DirectX::XMFLOAT3 scale) noexcept
{
	this->scale = scale;
}

DirectX::XMFLOAT3 GameObject::GetScale() const noexcept
{
	return scale;
}
