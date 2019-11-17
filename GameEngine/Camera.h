#pragma once
#include "Graphics.h"

class Camera
{
public:
	Camera(Graphics& gfx);
	DirectX::XMMATRIX GetViewMatrix() const noexcept;
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void Rotate(float dx, float dy) noexcept;
	void Translate(DirectX::XMFLOAT3 translation) noexcept;
	DirectX::XMFLOAT3 GetPos() const noexcept;
private:
	//float r = 20.0f; //distance to the origin
	//float theta = 0.0f;//rotation the camera around origin
	//float phi = 0.0f;
	//// rotation camera
	//float pitch = 0.0f;
	//float yaw = 0.0f;
	//float roll = 0.0f;
	Graphics& gfx;
	DirectX::XMFLOAT3 pos;
	float pitch;
	float yaw;
	static constexpr float travelSpeed = 12.0f;
	static constexpr float rotationSpeed = 0.004f;
};