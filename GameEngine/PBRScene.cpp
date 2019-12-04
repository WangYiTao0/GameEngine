#include "PBRScene.h"

PBRScene::PBRScene(Graphics& gfx)
	:
	gfx(gfx),
	Scene("PBRScene"),
	pbrObject(gfx, "Data\\Models\\PBR\\shaderBall.obj",0.02f)
{
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
}
