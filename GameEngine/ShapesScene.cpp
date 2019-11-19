#include "ShapesScene.h"


ShapesScene::ShapesScene(Graphics& gfx)
	:
	gfx(gfx),
	Scene("Shapes Scene"),
	gridTerrain(gfx,200,200,200,200)
{
	Role.SetRootTransform(DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f));
}

void ShapesScene::Update(float dt)
{

}

void ShapesScene::Draw()
{
	gridTerrain.Draw(gfx);

	Role.Draw(gfx);

	//Samba.Draw(gfx);

	//imgui
	gridTerrain.SpawnControlWindow(gfx);
	Role.ShowWindow(gfx, "Role");
	//Samba.ShowWindow(gfx, "Samba");
}
