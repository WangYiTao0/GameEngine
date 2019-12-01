#pragma once
#include "Drawable.h"
#include "GameObject.h"
#include "IndexedTriangleList.h"


class GridTerrain : public Drawable,public GameObject
{
public:
	GridTerrain(Graphics& gfx, float width = 256.0f, float depth = 256.0f,
		unsigned int m = 257, unsigned int n = 257,float gridSize = 5.0f);
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void Update(Graphics& gfx, float dt);
	void SpawnControlWindow(Graphics& gfx)noexcept;
private:
	float GetHillsHeight(float x, float z)const;
private:
	struct PSMaterialConstant
	{
		float specularIntensity = 0.1f;
		float specularPower = 20.0f;
		BOOL normalMappingEnabled = TRUE;
		float padding;
	} pmc;
	struct VSWaveConstant
	{
		DirectX::XMFLOAT3 pos;
		float padding;
	};
	
	VSWaveConstant* vwc = nullptr;

	static IndexedTriangleList model;

}; 
