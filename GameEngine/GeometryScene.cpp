#include "GeometryScene.h"
#include "DirectXMathHelper.h"

GeometryScene::GeometryScene(Graphics& gfx)
	:
	gfx(gfx),
	Scene("Geometry Scene"),
	sphere(gfx, 4.0f),
	rng(std::random_device{}())
{
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
}

void GeometryScene::Draw()
{
	using namespace DirectX;
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

	//imgui windows
	gridTerrain.SpawnControlWindow(gfx);
	SpownFrustumControl();

}

void GeometryScene::SpownFrustumControl()
{
	ImGui::Begin("Frustum");
	ImGui::Checkbox("EnableFrustumCulling", &EnableFrustumCulling);
	ImGui::Text("ObjectsCount: %s", std::to_string(refData.size()).c_str());
	ImGui::End();
}
