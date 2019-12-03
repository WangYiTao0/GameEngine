#pragma once
#include "Graphics.h"
#include "GameObject2D.h"
#include "GameObject3D.h"

class Camera3D : public GameObject3D
{
public:
	Camera3D(Graphics& gfx);
	void Set3DProj(float fov, float aspec, float nearZ, float farZ);
	DirectX::XMMATRIX GetProj();
	DirectX::XMMATRIX GetViewMatrix() const noexcept;

	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void Rotate(float dx, float dy) noexcept;
	void Translate(DirectX::XMFLOAT3 translation) noexcept;
private:
;
	Graphics& gfx;
	DirectX::XMMATRIX proj;
	static constexpr float travelSpeed = 12.0f;
	static constexpr float rotationSpeed = 0.004f;
};

class Camera2D
{
public:
	Camera2D();
	void SetOrtho(float width, float height, float nearZ, float farZ);
	void Translate(DirectX::XMFLOAT3 translation) noexcept;
	const DirectX::XMMATRIX& GetOrthoMatrix() const;
	DirectX::XMMATRIX GetWorldMatrix();

private:
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 rot;

	DirectX::XMMATRIX orthoMatrix;
};