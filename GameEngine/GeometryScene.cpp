#include "GeometryScene.h"

GeometryScene::GeometryScene(Graphics& gfx)
	:
	gfx(gfx),
	Scene("Geometry Scene"),
	plane(gfx, 3.0f),
	cube(gfx, 4.0f)
{
	plane.SetPos({ -5.0f,17.0f,-1.0f });
	cube.SetPos({ 3.0f,14.0f,-2.0f });
}

void GeometryScene::Update(Keyboard& kbd, Mouse& mouse, float dt)
{
}

void GeometryScene::Draw(Graphics& gfx) 
{	
	plane.Draw(gfx);
	cube.Draw(gfx);	

	//imgui windows
	plane.SpawnControlWindow(gfx);
	cube.SpawnControlWindow(gfx);
}
