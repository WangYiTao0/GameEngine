#pragma once
#include "Scene.h"
#include "Graphics.h"
#include "GridTerrain.h"
#include "Mesh.h"


class ShapesScene :public Scene
{
public:
	ShapesScene(Graphics& gfx);
	virtual void Update(float dt) override;
	virtual void Draw()  override;
private:
	Graphics& gfx;

};