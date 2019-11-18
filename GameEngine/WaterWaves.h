#pragma once
#include "Drawable.h"

class WaterWaves : public Drawable
{
public:
	WaterWaves(Graphics& gfx, float width = 256.0f, float depth = 256.0f,
		unsigned int m = 257, unsigned int n = 257, float gridSize = 5.0f);
	
	void Update(float dt);

	void Disturb(UINT i, UINT j, float magnitude);


private:
	float dt;
	float speed;
	float damping;

	UINT mNumRows;
	UINT mNumCols;

	UINT mVertexCount;
	UINT mTriangleCount;

	// Simulation constants we can precompute.
	float mK1;
	float mK2;
	float mK3;

	float mTimeStep;
	float mSpatialStep;

	DirectX::XMFLOAT3* mPrevSolution;
	DirectX::XMFLOAT3* mCurrSolution;
};
#pragma once
