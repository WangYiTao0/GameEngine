#include "ModelScene.h"
#include "App.h"

namespace dx = DirectX;

ModelScene::ModelScene(Graphics& gfx)
	:
	gfx(gfx),
	Scene("Model Scene")
{
	//wall.SetRootTransform(dx::XMMatrixTranslation(-12.0f, 0.0f, 0.0f));
	//tp.SetPos({ 12.0f,0.0f,0.0f });
	//gobber.SetRootTransform(dx::XMMatrixTranslation(0.0f, 0.0f, -4.0f));
	//nano.SetRootTransform(dx::XMMatrixTranslation(0.0f, -7.0f, 6.0f));

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
