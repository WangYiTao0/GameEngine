#pragma once
#include "Scene.h"
#include "Mesh.h"
#include "TestPlane.h"


class ModelScene : public Scene
{
public:
	ModelScene(Graphics& gfx);

	virtual void Update(float dt) override;

	virtual void Draw()override;
private:
	Graphics& gfx;

	Model sponza{ gfx,"Data\\Models\\sponza\\sponza.obj",1.0f / 20.0f };

	TestPlane bluePlane{ gfx,6.0f,{ 0.3f,0.3f,1.0f,0.0f } };
	TestPlane redPlane{ gfx,6.0f,{ 1.0f,0.3f,0.3f,0.0f } };
};