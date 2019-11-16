#include "ShapesScene.h"

ShapesScene::ShapesScene(Graphics& gfx)
	:
	gfx(gfx),
	Scene("Shapes Scene"),
	gridTerrain(gfx,200,200,200,200)
{
	
}

void ShapesScene::Update(float dt)
{
}

void ShapesScene::Draw()
{
	gridTerrain.Draw(gfx);


	//imgui
	gridTerrain.SpawnControlWindow(gfx);
}
