#include "ModelScene.h"
#include "App.h"
#include <DirectXTex.h>


namespace dx = DirectX;

ModelScene::ModelScene(Graphics& gfx)
	:
	gfx(gfx),
	Scene("Model Scene")
{
		bluePlane.SetPos({0.0f,0.0f,5.0f});
		redPlane.SetPos({ 0.0f,0.0f,6.0f });

	

	sponza.SetRootTransform(DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f)
		* DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f));
}

void ModelScene::Update(float dt)
{
	
}

void ModelScene::Draw() 
{

	sponza.Draw(gfx);

	redPlane.Draw(gfx);
	bluePlane.Draw(gfx);



	sponza.ShowWindow(gfx, "Sponza");
	bluePlane.SpawnControlWindow(gfx, "Blue Plane");
	redPlane.SpawnControlWindow(gfx, "Red Plane");



}
