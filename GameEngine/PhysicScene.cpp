#include "PhysicScene.h"

PhysicScene::PhysicScene(Graphics& gfx)
	:
	gfx(gfx),
	Scene("Physic Scene"),
	plane1(gfx,10.0f),
	cube1(gfx,4.0f),
	pObj(cube1.GetPos(),DirectX::XMFLOAT3(0.0f,-1.0f,0.0f))
{
	plane1.SetPos({ -5.0f,17.0f,-1.0f });
	cube1.SetPos({ 3.0f,14.0f,-2.0f });
}

void PhysicScene::Update(float dt)
{
}

void PhysicScene::Draw()
{
	plane1.Draw(gfx);
	cube1.Draw(gfx);

	//imgui windows
	plane1.SpawnControlWindow(gfx);
	cube1.SpawnControlWindow(gfx);

}
