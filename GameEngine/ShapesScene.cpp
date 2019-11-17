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

	armory_Blast_W_HIGH.Draw(gfx);

	//Samba.Draw(gfx);

	//imgui
	gridTerrain.SpawnControlWindow(gfx);
	armory_Blast_W_HIGH.ShowWindow(gfx, "armory_Blast_W_HIGH");
	//Samba.ShowWindow(gfx, "Samba");
}
