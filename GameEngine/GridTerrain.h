#pragma once
#include "Drawable.h"

class GridTerrain : public Drawable 
{
public:
	GridTerrain(Graphics& gfx, float width = 24.0f, float depth = 24.0f,
		unsigned int m = 24, unsigned int n = 24);
	void SetPos(DirectX::XMFLOAT3 pos) noexcept;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	DirectX::XMFLOAT3 pos = { 1.0f,1.0f,1.0f };
}; 
