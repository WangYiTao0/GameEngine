#pragma once
#include "Drawable.h"
#include "GameObject.h"
class TestPlane : public Drawable, public GameObject
{
public:
	TestPlane(Graphics& gfx, float size, DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,0.0f });
	virtual DirectX::XMMATRIX GetTransformXM()const noexcept override;
	void SpawnControlWindow(Graphics& gfx, const std::string& name) noexcept;
private:
	struct PSMaterialConstant
	{
		DirectX::XMFLOAT4 color;
	} pmc;
};