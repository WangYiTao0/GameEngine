#include "ModelScene.h"

ModelScene::ModelScene(Graphics& gfx)
	:
	gfx(gfx),
	Scene("Model Scene")
{
}

void ModelScene::Update(float dt)
{
}

void ModelScene::Draw() 
{
	//nano.Draw(gfx);
	//nano2.Draw(gfx);
	minecraft.Draw(gfx);

	//imgui windows
	//nano.ShowWindow("Model 1");
	//nano2.ShowWindow("Model 2");
	minecraft.ShowWindow("Minecraft 1");
}
