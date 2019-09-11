#pragma once
#include "Scene.h"
#include "TestCube.h"
#include "TestPlane.h"
#include "PhysicsObject.h"

class PhysicScene : public Scene
{
public:
	PhysicScene(Graphics& gfx);

	virtual void Update(float dt) override;

	virtual void Draw()override;
private:
	Graphics& gfx;

	TestCube cube1;
	TestPlane plane1;

	PhysicsObject pObj;
};