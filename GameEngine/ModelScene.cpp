#include "ModelScene.h"
#include "App.h"

ModelScene::ModelScene(Graphics& gfx)
	:
	gfx(gfx),
	Scene("Model Scene")
{
	//wall.SetRootTransform(DirectX::XMMatrixTranslation(-1.5f, 0.0f, 0.0f));
	//tp.SetPos({ 1.5f,0.0f,0.0f });
}

void ModelScene::Update(float dt)
{
	
}

void ModelScene::Draw() 
{

	gobber.Draw(gfx);

	gobber.ShowWindow(gfx,"gobber");

}
