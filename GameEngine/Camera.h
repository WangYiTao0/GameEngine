#pragma once
#include "Graphics.h"

class Camera
{
public:
	DirectX::XMMATRIX GetMatrix() const noexcept;
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
private:
	float r = 20.0f; //distance to the origin
	float theta = 0.0f;//rotation the camera around origin
	float phi = 0.0f;
	// rotation camera
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;
};