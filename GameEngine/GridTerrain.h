#pragma once
#include "Drawable.h"

class GridTerrain : public Drawable 
{
public:
	GridTerrain(Graphics& gfx, float width = 24.0f, float depth = 24.0f,
		unsigned int m = 24, unsigned int n = 24);
	void SetPos(DirectX::XMFLOAT3 pos) noexcept;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void SpawnControlWindow(Graphics& gfx)noexcept;
private:
	struct PSMaterialConstant
	{
		float specularIntensity = 0.1f;
		float specularPower = 20.0f;
		BOOL normalMappingEnabled = TRUE;
		float padding[1];
	} pmc;

	DirectX::XMFLOAT3 pos = { 1.0f,1.0f,1.0f };
}; 
