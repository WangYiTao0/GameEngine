#pragma once
#include "Drawable.h"
#include "GameObject.h"
#include "Waves.h"
#include "IndexedTriangleList.h"

class WaterWave:public Drawable,public GameObject
{
public:
	WaterWave(Graphics& gfx);
	void Update(float dt);
	virtual DirectX::XMMATRIX GetTransformXM()const noexcept override;
private:
	Waves mWaves;
	static IndexedTriangleList wave;
};