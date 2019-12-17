#pragma once
#include "Drawable.h"
#include "GameObject.h"
#include "IndexedTriangleList.h"
#include "LightCommon.h"


class GridTerrain : public Drawable,public GameObject
{
public:
	GridTerrain(Graphics& gfx, float width = 256.0f, float depth = 256.0f,
		unsigned int m = 257, unsigned int n = 257,float gridSize = 5.0f);
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void Update(Graphics& gfx, float dt);
	void DrawDepth(Graphics& gfx)noexcept;
	void SpawnControlWindow(Graphics& gfx)noexcept;
private:
	float GetHillsHeight(float x, float z)const;
private:
	Material pmc;
	static IndexedTriangleList model;
	std::vector<std::shared_ptr<Bind::Bindable>> depths;

}; 
