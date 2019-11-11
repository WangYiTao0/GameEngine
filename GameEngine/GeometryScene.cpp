#include "GeometryScene.h"

namespace DirectX
{
	bool XM_CALLCONV operator == (XMFLOAT4X4& lhs, XMFLOAT4X4& rhs)
	{
		if (lhs._11 == rhs._11 || lhs._12 == rhs._12 || lhs._13, rhs._13 || lhs._14 == rhs._14
			|| lhs._21 == rhs._21 || lhs._22 == rhs._22 || lhs._23, rhs._23 || lhs._24 == rhs._24
			|| lhs._31 == rhs._31 || lhs._32 == rhs._32 || lhs._33, rhs._33 || lhs._34 == rhs._34
			|| lhs._41 == rhs._41 || lhs._42 == rhs._42 || lhs._43, rhs._43 || lhs._44 == rhs._44)
		{
			return true;
		}
		return false;
	}

	bool XM_CALLCONV operator == (XMMATRIX& lhs, XMMATRIX& rhs)
	{	
		XMFLOAT4X4 flhs = {};
		XMFLOAT4X4 frhs = {};
		XMStoreFloat4x4(&flhs, lhs);
		XMStoreFloat4x4(&frhs, rhs);

		if (flhs == frhs)
		{
			return true;
		}

		return false;
	}


}

GeometryScene::GeometryScene(Graphics& gfx)
	:
	gfx(gfx),
	Scene("Geometry Scene"),
	plane(gfx, 3.0f),
	sphere(gfx, 4.0f),
	rng(std::random_device{}())
{
	plane.SetPos({ 0.0f,0.0f,5.0f });
	plane.SetRotation(0.0f, 0.0f, 0.0f);

	std::uniform_real_distribution<float> rdist(-20.0f, 20.0f);

	frustum = std::make_unique<Frustum>();

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
	//XMFLOAT4X4 flhs = {};
	//XMFLOAT4X4 frhs = {};
	//flhs == frhs;
	XMMATRIX lhs, rhs;
	//lhs == rhs;

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
			gfx.GetCamera(),
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
