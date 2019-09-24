#pragma once
#include "Scene.h"
#include "TestPlane.h"
#include "TestCube.h"

class GeometryScene : public Scene
{
public:
	GeometryScene(Graphics& gfx);

	virtual void Update(float dt) override;

	virtual void Draw()  override;
private:
	Graphics& gfx;
	//TestPlane plane;
	//TestCube cube;
};