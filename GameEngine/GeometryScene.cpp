#include "GeometryScene.h"
#include "DirectXMathHelper.h"

GeometryScene::GeometryScene(Graphics& gfx)
	:
	gfx(gfx),
	Scene("Geometry Scene")
{
	cube.SetPos({ 0.0f,2.0f,0.0f });
}

void GeometryScene::Update(float dt)
{
	gridTerrain.Update(gfx, dt);


	gridTerrain.SpawnControlWindow(gfx);
	cube.SpawnControlWindow(gfx);
}

void GeometryScene::Draw()
{
	gridTerrain.Draw(gfx);
	cube.Draw(gfx);



}

void GeometryScene::SpownFrustumControl()
{

}
