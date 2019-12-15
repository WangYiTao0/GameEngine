#include "GeometryScene.h"
#include "DirectXMathHelper.h"

GeometryScene::GeometryScene(Graphics& gfx)
	:
	gfx(gfx),
	rng(std::random_device{}()),
	Scene("Geometry Scene")

{
	std::uniform_real_distribution<float> rdist(-20.0f, 20.0f);


	//role = std::make_unique<Model>(gfx, "Data\\Models\\Role\\ascendant_one_iris\\iris.obj", 2.0f);
	//role->AddShader(gfx, "PhongVSTBN", "PhongPSSpecNormalMap");
	std::vector<std::string> filePath = {
		"Data\\Images\\skybox\\sunset_posX.bmp", "Data\\Images\\skybox\\sunset_negX.bmp",
		"Data\\Images\\skybox\\sunset_posY.bmp", "Data\\Images\\skybox\\sunset_negY.bmp",
		"Data\\Images\\skybox\\sunset_posZ.bmp", "Data\\Images\\skybox\\sunset_negZ.bmp" };

	for (auto i = 0; i < 1; i++)
	{
		cubes.push_back(std::make_unique<TestCube>(gfx, 2.0f));
	}

	for (auto& c : cubes)
	{
		//c->SetPos({ rdist(rng),rdist(rng),rdist(rng) });
		c->SetPos({ 0,1,0 });
		//BoxData.push_back(c->GetLocalBoundingBox());
		//m_InstancedData.push_back(c->GetTransformXM());

	}
	m_pSunset = std::make_unique<SkyRender>(gfx,filePath,1000.0f);

}

void GeometryScene::Update(float dt)
{
	gridTerrain.Update(gfx, dt);


	gridTerrain.SpawnControlWindow(gfx);
	//cube.SpawnControlWindow(gfx,"cube");
}

void GeometryScene::Draw()
{
	gridTerrain.DrawIndexed(gfx);

	for (auto& b : cubes)
	{
		b->DrawIndexed(gfx);
		b->DrawOutline(gfx);
	}
	//role->Draw(gfx);


	m_pSunset->DrawIndexed(gfx);
}

void GeometryScene::DrawDepth()
{
	for (auto& b : cubes)
	{
		b->DrawDepth(gfx);
	}
}

void GeometryScene::SpownFrustumControl()
{

}
