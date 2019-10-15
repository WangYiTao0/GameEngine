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

	//Model gobber{ gfx,"Models\\gobber\\GoblinX.obj",6.0f };
	//Model wall{ gfx,"Models\\brick_wall\\brick_wall.obj",6.0f };
	//TestPlane tp{ gfx,6.0 };
	//Model nano{ gfx,"Models\\nano_textured\\nanosuit.obj",2.0f };

	Model sponza{ gfx,"Models\\sponza\\sponza.obj",1.0f / 20.0f };

};