#include "ShapesScene.h"

ShapesScene::ShapesScene(Graphics& gfx)
	:
	gfx(gfx),
	Scene("Shapes Scene"),
	gridTerrain(gfx)
{
	
}

void ShapesScene::Update(float dt)
{
}

void ShapesScene::Draw()
{
	gridTerrain.Draw(gfx);
}
