#include "GeometryScene.h"
#include "DirectXMathHelper.h"

GeometryScene::GeometryScene(Graphics& gfx)
	:
	gfx(gfx),
	Scene("Geometry Scene"),
	plane(gfx, 3.0f),
	sphere(gfx, 4.0f),
	gridTerrain(gfx),
	rng(std::random_device{}())
{
	plane.SetPos({ 0.0f,0.0f,5.0f });
	plane.SetRotation(0.0f, 0.0f, 0.0f);

	std::uniform_real_distribution<float> rdist(-20.0f, 20.0f);

	for (auto i = 0; i < 80; i++)
	{
		cubes.push_back(std::make_unique<TestCube>(gfx, 2.0f));
	}

	for (auto& c : cubes)
	{
		c->SetPos({ rdist(rng),rdist(rng),rdist(rng) });
		//BoxData.push_back(c->GetLocalBoundingBox());
		m_InstancedData.push_back(c->GetTransformXM());

	}
}

void GeometryScene::Update(float dt)
{
	//frustum->BuildFrustum(4000.0f, gfx.GetProjection(), gfx.GetCamera());
	//for (auto& c : cubes)
	//{
	//	if (EnableFrustumCulling)
	//	{
	//		isVisiable = frustum->CheckCube(c->GetPos().x, c->GetPos().y, c->GetPos().z, 1.0f);
	//	}
	//	else
	//	{
	//		isVisiable = true;
	//	}
	//}

}

void GeometryScene::Draw()
{
	using namespace DirectX;
	plane.Draw(gfx);
	//sphere.Draw(gfx);
	//actually draw number
	std::vector<DirectX::XMMATRIX> acceptedData;
	for (auto& c : cubes)
	{
		c->GetLocalBoundingBox();
	}

	auto B = DirectX::BoundingBox();
	 
	if (EnableFrustumCulling)
	{
		acceptedData = Collision::FrustumCulling(
			m_InstancedData,
			B,
			gfx.GetCameraViewMatrix(),
			gfx.GetProjection());	
	}
	
	//data 
	refData = EnableFrustumCulling ? acceptedData : m_InstancedData;

	for (auto& c : cubes)
	{
		auto CubeMat = c->GetTransformXM();
		for (XMMATRIX mat : refData)
		{
			if (mat == CubeMat)
			{
				c->Draw(gfx);
			}
			
		}
	}

	gridTerrain.Draw(gfx);

	//for (auto& c : cubes)
	//{
	//	if (isVisiable)
	//	{
	//		c->Draw(gfx);
	//		renderCount++;
	//	}
	//}


	//imgui windows
	plane.SpawnControlWindow(gfx);
	SpownFrustumControl();

}

void GeometryScene::SpownFrustumControl()
{
	ImGui::Begin("Frustum");
	ImGui::Checkbox("EnableFrustumCulling", &EnableFrustumCulling);
	ImGui::Text("ObjectsCount: %s", std::to_string(refData.size()).c_str());
	ImGui::End();
}
