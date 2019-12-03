#pragma once
#include "Drawable.h"
#include "GameObject.h"

#include <DirectXCollision.h>
class TestCube : public Drawable, public GameObject
{
public:
	TestCube(Graphics& gfx, float size);
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
	DirectX::BoundingBox boundingBox;

};