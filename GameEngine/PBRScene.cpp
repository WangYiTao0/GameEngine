#include "PBRScene.h"

PBRScene::PBRScene(Graphics& gfx)
	:
	gfx(gfx),
	Scene("PBRScene"),
	pbrObject(gfx, "Data\\Models\\PBR\\shaderBall.obj",0.02f)
{
	std::vector<std::string> files = {
		"Data\\Images\\PBR\\cgbookcase\\black-herringbone-tiles-01\\Black_herringbone_tiles_01_2K_Base_Color.png",
		"Data\\Images\\PBR\\cgbookcase\\black-herringbone-tiles-01\\Black_herringbone_tiles_01_2K_Height.png",
		"Data\\Images\\PBR\\cgbookcase\\black-herringbone-tiles-01\\Black_herringbone_tiles_01_2K_Normal.png",
		"Data\\Images\\PBR\\cgbookcase\\black-herringbone-tiles-01\\Black_herringbone_tiles_01_2K_Roughness.png"
	};
	pbrObject.AddPBRTexture(gfx, files);
	pbrObject.AddShader(gfx, "Forward_BRDF_VS", "Forward_BRDF_PS");
}

PBRScene::~PBRScene()
{
}

void PBRScene::Update(float dt)
{
}

void PBRScene::Draw()
{
	pbrObject.Draw(gfx);
	//m_pSunset->DrawIndexed(gfx);
	//gridTerrain.DrawIndexed(gfx);
}
