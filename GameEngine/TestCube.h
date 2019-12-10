#pragma once
#include "Drawable.h"
#include "GameObject.h"
#include "LightCommon.h"
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
	Material pmc;
	DirectX::BoundingBox boundingBox;

};