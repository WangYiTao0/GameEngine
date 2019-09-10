#include "ModelScene.h"



ModelScene::ModelScene(Graphics& gfx)
	:
	gfx(gfx),
	Scene("Model Scene")
{
}

void ModelScene::Update(Keyboard& kbd, Mouse& mouse, float dt)
{
}

void ModelScene::Draw(Graphics& gfx) 
{
	nano.Draw(gfx);
	nano2.Draw(gfx);

	//imgui windows
	nano.ShowWindow("Model 1");
	nano2.ShowWindow("Model 2");
}
