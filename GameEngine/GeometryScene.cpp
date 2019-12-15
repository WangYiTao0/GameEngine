#include "GeometryScene.h"
#include "DirectXMathHelper.h"

GeometryScene::GeometryScene(Graphics& gfx)
	:
	gfx(gfx),
	Scene("Geometry Scene")
{
	//role = std::make_unique<Model>(gfx, "Data\\Models\\Role\\ascendant_one_iris\\iris.obj", 2.0f);
	//role->AddShader(gfx, "PhongVSTBN", "PhongPSSpecNormalMap");
	std::vector<std::string> filePath = {
		"Data\\Images\\skybox\\sunset_posX.bmp", "Data\\Images\\skybox\\sunset_negX.bmp",
		"Data\\Images\\skybox\\sunset_posY.bmp", "Data\\Images\\skybox\\sunset_negY.bmp",
		"Data\\Images\\skybox\\sunset_posZ.bmp", "Data\\Images\\skybox\\sunset_negZ.bmp" };

	cube.SetPos({ 0.0f,2.0f,0.0f });	
	m_pSunset = std::make_unique<SkyRender>(gfx,filePath,1000.0f);

}

void GeometryScene::Update(float dt)
{
	gridTerrain.Update(gfx, dt);


	gridTerrain.SpawnControlWindow(gfx);
	cube.SpawnControlWindow(gfx,"cube");
}

void GeometryScene::Draw()
{
	gridTerrain.DrawIndexed(gfx);
	cube.DrawIndexed(gfx);
	cube.DrawOutline(gfx);
	//role->Draw(gfx);


	m_pSunset->DrawIndexed(gfx);
}

void GeometryScene::DrawDepth()
{
	cube.DrawDepth(gfx);
}

void GeometryScene::SpownFrustumControl()
{

}
