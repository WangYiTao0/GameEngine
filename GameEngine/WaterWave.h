#pragma once
#include "Drawable.h"
#include "GameObject3D.h"
#include "Waves.h"
#include "IndexedTriangleList.h"

class WaterWave:public Drawable,public GameObject3D
{
public:
	WaterWave(Graphics& gfx);
	void Update(float dt);
	virtual DirectX::XMMATRIX GetTransformXM()const noexcept override;
private:
	Waves mWaves;
	static IndexedTriangleList wave;
};