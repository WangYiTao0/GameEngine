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
	void SpawnControlWindow(Graphics& gfx, const char* name) noexcept;
	DirectX::BoundingBox GetLocalBoundingBox() noexcept;

	void DrawDepth(Graphics& gfx)noexcept;
private:
	void CreateBoundingBox();
private:
	std::vector<std::shared_ptr<Bind::Bindable>> depth;

	std::vector<std::shared_ptr<Bind::Bindable>> outlineEffect;
	Material pmc;
	DirectX::BoundingBox boundingBox;
};