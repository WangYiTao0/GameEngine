#include "ModelScene.h"
#include "App.h"
#include <DirectXTex.h>


namespace dx = DirectX;

ModelScene::ModelScene(Graphics& gfx)
	:
	gfx(gfx),
	Scene("Model Scene")
{
	std::vector<std::string> filePath = {
	"Data\\Images\\skybox\\sunset_posX.bmp", "Data\\Images\\skybox\\sunset_negX.bmp",
	"Data\\Images\\skybox\\sunset_posY.bmp", "Data\\Images\\skybox\\sunset_negY.bmp",
	"Data\\Images\\skybox\\sunset_posZ.bmp", "Data\\Images\\skybox\\sunset_negZ.bmp" };
	m_pSunset = std::make_unique<SkyRender>(gfx, filePath, 1000.0f);


	//bluePlane.SetPos({0.0f,0.0f,5.0f});
	//redPlane.SetPos({ 0.0f,0.0f,6.0f });
	sponza.AddPixelShader(gfx, "PhongPSSpecNormalMap");
	sponza.SetRootTransform(DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f)
		* DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f));	

}

void ModelScene::Update(float dt)
{
	
	//imgui window in update 
	//conflict with RTT
	sponza.ShowWindow(gfx, "Sponza");
	//bluePlane.SpawnControlWindow(gfx, "Blue Plane");
	//redPlane.SpawnControlWindow(gfx, "Red Plane");
}

void ModelScene::Draw() 
{
	sponza.Draw(gfx);
	//redPlane.DrawIndexed(gfx);
	//bluePlane.DrawIndexed(gfx);


	m_pSunset->DrawIndexed(gfx);
}
