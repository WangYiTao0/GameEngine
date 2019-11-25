#pragma once
#include "Drawable.h"
#include "GameObject.h"

class WaterWaves : public Drawable,public GameObject
{
public:
	WaterWaves(Graphics& gfx, float width = 256.0f, float depth = 256.0f,
		unsigned int m = 257, unsigned int n = 257, float gridSize = 5.0f);
	
	void Update(float dt);
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:

//	IndexedTriangleList model;

	
};
#pragma once
