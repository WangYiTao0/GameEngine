#pragma once
#include "Drawable.h"

class WaterWaves : public Drawable
{
public:
	WaterWaves(Graphics& gfx, float width = 256.0f, float depth = 256.0f,
		unsigned int m = 257, unsigned int n = 257, float gridSize = 5.0f);
	
	void Update(float dt);

	void SetPos(DirectX::XMFLOAT3 pos) noexcept;


	virtual DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:

	//IndexedTriangleList model;

	DirectX::XMFLOAT3 pos = { 1.0f,1.0f,1.0f };
};
#pragma once
