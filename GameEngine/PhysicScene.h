#pragma once
#include "Scene.h"
#include "Graphics.h"
#include "Cube.h"

class PhysicScene : public Scene
{
public:
	PhysicScene(Graphics& gfx);
	~PhysicScene();

	virtual void Update(float dt) override;

	virtual void Draw()override;
private:
	Graphics& gfx;



};