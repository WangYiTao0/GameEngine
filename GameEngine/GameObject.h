#pragma once
#include <DirectXMath.h>


class GameObject
{
public:
	void SetPos(DirectX::XMFLOAT3 pos)noexcept;
	DirectX::XMFLOAT3 GetPos()const noexcept;
	void SetRotation(DirectX::XMFLOAT3 rollPitchYaw) noexcept;
	DirectX::XMFLOAT3 GetRotation()const noexcept;
	void SetScale(DirectX::XMFLOAT3 rollPitchYaw) noexcept;
	DirectX::XMFLOAT3 GetScale()const noexcept;
protected:
	virtual void UpdateMatrix();

protected:
	/*
	YawÅFZ
	RollÅFX
	PitchÅFY
	*/
	DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 rollPitchYaw = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
};