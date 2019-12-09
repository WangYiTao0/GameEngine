#include "GeometryScene.h"
#include "DirectXMathHelper.h"

GeometryScene::GeometryScene(Graphics& gfx)
	:
	gfx(gfx),
	Scene("Geometry Scene")
{
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
	cube.SpawnControlWindow(gfx);
}

void GeometryScene::Draw()
{
	gridTerrain.DrawIndexed(gfx);
	cube.DrawIndexed(gfx);

	m_pSunset->DrawIndexed(gfx);



}

void GeometryScene::SpownFrustumControl()
{

}
