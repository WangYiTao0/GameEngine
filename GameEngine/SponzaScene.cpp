#include "SponzaScene.h"
#include "App.h"
#include <DirectXTex.h>


namespace dx = DirectX;

SponzaScene::SponzaScene(Graphics& gfx)
	:
	gfx(gfx),
	Scene("Model Scene")
{
	// init Light
	m_Light = std::make_unique<Light>(gfx,"SponzaScene",1, 4, 4);


	sponza=std::make_unique<Model>( gfx, "Data\\Models\\sponza\\sponza.obj", 1.0f / 20.0f );

	//bluePlane.SetPos({0.0f,0.0f,5.0f});
	//redPlane.SetPos({ 0.0f,0.0f,6.0f });

	//sponza->AddShader(gfx,"PhongVSTBN", "PhongPSSpecNormalMap");
	//sponza->SetRootTransform(DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f)
	//	* DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f));	

	std::vector<std::string> filePath = {
		"Data\\Images\\skybox\\sunset\\sunset_posX.bmp", "Data\\Images\\skybox\\sunset\\sunset_negX.bmp",
		"Data\\Images\\skybox\\sunset\\sunset_posY.bmp", "Data\\Images\\skybox\\sunset\\sunset_negY.bmp",
		"Data\\Images\\skybox\\sunset\\sunset_posZ.bmp", "Data\\Images\\skybox\\sunset\\sunset_negZ.bmp" };

	m_pSunset = std::make_unique<SkyRender>(gfx, filePath, 1000.0f);
}

void SponzaScene::Update(float dt)
{

	//update light
	m_Light->Update(gfx);
	m_Light->Bind(gfx);

	//imgui window in update 
	//conflict with RTT
	sponza->ShowWindow(gfx, "Sponza");
	//bluePlane.SpawnControlWindow(gfx, "Blue Plane");
	//redPlane.SpawnControlWindow(gfx, "Red Plane");

	m_Light->SpawnLightManagerWindow(gfx);
}

void SponzaScene::Draw() 
{
	m_Light->Draw(gfx);

	sponza->Draw(gfx);


	m_pSunset->DrawIndexed(gfx);
	//redPlane.DrawIndexed(gfx);
	//bluePlane.DrawIndexed(gfx);
}
