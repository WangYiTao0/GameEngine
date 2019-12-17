#include "GeometryScene.h"
#include "DirectXMathHelper.h"
#include "MathHelper.h"
GeometryScene::GeometryScene(Graphics& gfx)
	:
	gfx(gfx),
	rng(std::random_device{}()),
	Scene("Geometry Scene")

{
	std::uniform_real_distribution<float> rdist(-10.0f, 10.0f);
	std::uniform_real_distribution<float> rdistY(2.0f, 10.f);
	std::uniform_real_distribution<float> rRot(-MH::PI,MH::PI);

	//role = std::make_unique<Model>(gfx, "Data\\Models\\nano_textured\\nanosuit.obj", 1.0f);
	//role->AddShader(gfx, "PhongVSTBN", "PhongPSSpecNormalMap");
	std::vector<std::string> filePath = {
		"Data\\Images\\skybox\\sunset\\sunset_posX.bmp", "Data\\Images\\skybox\\sunset\\sunset_negX.bmp",
		"Data\\Images\\skybox\\sunset\\sunset_posY.bmp", "Data\\Images\\skybox\\sunset\\sunset_negY.bmp",
		"Data\\Images\\skybox\\sunset\\sunset_posZ.bmp", "Data\\Images\\skybox\\sunset\\sunset_negZ.bmp" };

	for (auto i = 0; i < 10; i++)
	{
		cubes.push_back(std::make_unique<TestCube>(gfx, 2.0f));
	}

	for (auto& c : cubes)
	{
		c->SetPos({ rdist(rng),2,rdist(rng) });
	}
	m_pSunset = std::make_unique<SkyRender>(gfx,filePath,1000.0f);

}

void GeometryScene::Update(float dt)
{
	gridTerrain.Update(gfx, dt);


	gridTerrain.SpawnControlWindow(gfx);
//	role->ShowWindow(gfx, "role");
	for (auto& c : cubes)
	{
		c->SpawnControlWindow(gfx, "cube");
	}
}

void GeometryScene::Draw()
{
	gridTerrain.DrawIndexed(gfx);

	for (auto& b : cubes)
	{
		b->DrawIndexed(gfx);
		//b->DrawOutline(gfx);
	}
//	role->Draw(gfx);


	m_pSunset->DrawIndexed(gfx);
}

void GeometryScene::DrawDepth()
{
	for (auto& b : cubes)
	{
		b->DrawDepth(gfx);
	}
	gridTerrain.DrawDepth(gfx);
}

void GeometryScene::SpownFrustumControl()
{

}
