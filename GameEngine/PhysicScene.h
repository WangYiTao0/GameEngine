#pragma once
#include "Scene.h"
#include "Graphics.h"
#include "Component.h"

class PhysicScene : public Scene
{
public:
	PhysicScene(Graphics& gfx);

	virtual void Update(float dt) override;

	virtual void Draw()override;
private:
	Graphics& gfx;

	ECS ecs;
};