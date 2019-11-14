#include "ModelScene.h"
#include "App.h"

namespace dx = DirectX;

ModelScene::ModelScene(Graphics& gfx)
	:
	gfx(gfx),
	Scene("Model Scene")
{

	sponza.SetRootTransform(DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f)
		* DirectX::XMMatrixRotationRollPitchYaw(0.0f, 80.0f, 0.0f));
}

void ModelScene::Update(float dt)
{
	
}

void ModelScene::Draw() 
{
	//wall.Draw(gfx);
	//tp.Draw(gfx);
	//nano.Draw(gfx);
	//gobber.Draw(gfx);
	sponza.Draw(gfx);

	sponza.ShowWindow(gfx, "Sponza");

	//gobber.ShowWindow(gfx,"gobber");
	//wall.ShowWindow(gfx, "Wall");
	//tp.SpawnControlWindow(gfx);
	//nano.ShowWindow(gfx, "Nano");

}
