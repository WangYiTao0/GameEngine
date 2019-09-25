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

	Model gobber{ gfx,"Models\\gobber\\GoblinX.obj" };
	//Model gobber{ gfx,"Models\\body\\body2.80.fbx" };
	//Model nano1 { gfx,"Models\\nano_textured\\nanosuit.obj" };
	//Model nano2{ gfx,"Models\\nano_textured\\nanosuit.obj" };
};