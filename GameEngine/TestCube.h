#pragma once
#include "Drawable.h"
#include <DirectXCollision.h>
class TestCube : public Drawable
{
public:
	TestCube(Graphics& gfx, float size);
	void SetPos(DirectX::XMFLOAT3 pos) noexcept;
	DirectX::XMFLOAT3 GetPos()noexcept;
	void SetRotation(float roll, float pitch, float yaw) noexcept;
	void SetWorldMatrix(DirectX::XMMATRIX m_worldMatrix);
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void SpawnControlWindow(Graphics& gfx) noexcept;

	DirectX::BoundingBox GetLocalBoundingBox() noexcept;
private:
	void CreateBoundingBox();
	

private:
	struct PSMaterialConstant
	{
		float specularIntensity = 0.1f;
		float specularPower = 20.0f;
		BOOL normalMappingEnabled = TRUE;
		float padding[1];
	} pmc;

	DirectX::XMMATRIX m_worldMatrix;

	DirectX::XMFLOAT3 pos = { 1.0f,1.0f,1.0f };
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float size = 0.0f;

	DirectX::BoundingBox boundingBox;

};