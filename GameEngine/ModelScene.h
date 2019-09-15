#pragma once
#include "Scene.h"
#include "Mesh.h"

class ModelScene : public Scene
{
public:
	ModelScene(Graphics& gfx);

	virtual void Update(float dt) override;

	virtual void Draw()override;
private:
	Graphics& gfx;

	//Model nano1 { gfx,"Models\\nano_textured\\nanosuit.obj" };
//	Model nano2{ gfx,"Models\\nano_textured\\nanosuit.obj" };

	Model wall{ gfx,"Models\\brick_wall\\brick_wall.obj" };

	//ModelWithOutTexture nano3{ gfx, "Models\\nano_hierarchy.gltf" };
	//ModelWithOutTexture nano4{ gfx, "Models\\body\\body2.80.fbx" };
};