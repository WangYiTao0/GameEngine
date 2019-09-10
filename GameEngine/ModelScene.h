#pragma once
#include "Scene.h"
#include "Mesh.h"

class ModelScene : public Scene
{
public:
	ModelScene(Graphics& gfx);

	virtual void Update(Keyboard& kbd, Mouse& mouse, float dt) override;

	virtual void Draw(Graphics& gfx)  override;
private:
	
	Graphics& gfx;

	Model nano { gfx,"Models\\nano_textured\\nanosuit.obj" };
	Model nano2{ gfx,"Models\\nano_textured\\nanosuit.obj" };
};