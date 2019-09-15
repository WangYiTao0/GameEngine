#include "ModelScene.h"
#include "App.h"

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

	//nano1.Draw(gfx);
	//nano2.Draw(gfx);
	//nano3.Draw(gfx);
	//nano4.Draw(gfx);

	wall.Draw(gfx);

	wall.ShowWindow("wall");

	//imgui windows
	//nano1.ShowWindow("Model 1");
	//nano2.ShowWindow("Model 2");
	//nano3.ShowWindow("Model 3");
	//nano4.ShowWindow("model 4");

}
