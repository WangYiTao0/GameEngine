#pragma once
#include "Scene.h"
#include "TestPlane.h"
#include "TestCube.h"

class GeometryScene : public Scene
{
public:
	GeometryScene(Graphics& gfx);

	virtual void Update(Keyboard& kbd, Mouse& mouse, float dt) override;

	virtual void Draw(Graphics& gfx)  override;
private:
	Graphics& gfx;
	TestPlane plane;
	TestCube cube;
};