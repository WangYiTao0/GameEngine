#include "GeometryScene.h"
#include "DirectXMathHelper.h"
#include "MathHelper.h"
GeometryScene::GeometryScene(Graphics& gfx, FrameCommander& fc)
	:
	gfx(gfx),
	fc(fc),
	rng(std::random_device{}()),
	Scene("Geometry Scene")

{
	// init Light
	m_Light = std::make_unique<Light>(gfx, "GeometryScene", 1, 4, 1);


	std::uniform_real_distribution<float> rdist(-25.0f, 25.0f);
	std::uniform_real_distribution<float> rdistY(2.0f, 10.f);
	std::uniform_real_distribution<float> rRot(-MH::PI,MH::PI);

	//role->AddShader(gfx, "PhongVSTBN", "PhongPSSpecNormalMap");
	std::vector<std::string> filePath = {
		"Data\\Images\\skybox\\sunset\\sunset_posX.bmp", "Data\\Images\\skybox\\sunset\\sunset_negX.bmp",
		"Data\\Images\\skybox\\sunset\\sunset_posY.bmp", "Data\\Images\\skybox\\sunset\\sunset_negY.bmp",
		"Data\\Images\\skybox\\sunset\\sunset_posZ.bmp", "Data\\Images\\skybox\\sunset\\sunset_negZ.bmp" };

//	std::vector<std::string> filePath = {
//"Data\\Images\\skybox\\Yokohama3\\posx.jpg", "Data\\Images\\skybox\\Yokohama3\\negx.jpg",
//"Data\\Images\\skybox\\Yokohama3\\posy.jpg", "Data\\Images\\skybox\\Yokohama3\\negy.jpg",
//"Data\\Images\\skybox\\Yokohama3\\posz.jpg", "Data\\Images\\skybox\\Yokohama3\\negz.jpg" };

	cube.SetPos({ 4.0f,0.0f,0.0f });
	cube2.SetPos({ 0.0f,4.0f,0.0f });

	sky = std::make_unique<SkyRender>(gfx,filePath,1000.0f);

}

GeometryScene::~GeometryScene()
{
	
}

void GeometryScene::Update(float dt)
{
	//update light
	m_Light->Update(gfx);
	m_Light->Bind(gfx);

	gridTerrain.Update(gfx, dt);

	gridTerrain.SpawnControlWindow(gfx);

	cube.SpawnControlWindow(gfx, "Cube 1");
	cube2.SpawnControlWindow(gfx, "Cube 2");


	m_Light->SpawnLightManagerWindow(gfx);
}

void GeometryScene::Draw()
{	
	
	m_Light->Submit(fc);
	gridTerrain.Submit(fc);

	cube.Submit(fc);
	cube2.Submit(fc);

	//sky->Submit(fc);

	fc.Execute(gfx);

}

void GeometryScene::DrawDepth()
{
	//for (auto& b : cubes)
	//{
	//	b->DrawDepth(gfx);
	//}
	//gridTerrain.DrawDepth(gfx);
}

void GeometryScene::SpownFrustumControl()
{

}
