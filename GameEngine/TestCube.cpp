#include "TestCube.h"
#include "Cube.h"
#include "BindableCommon.h"
#include "TransformPixelCbuf.h"
#include "imgui/imgui.h"
#include "StringHelper.h"

TestCube::TestCube(Graphics& gfx, float size)
{
	this->scale = { size,size,size };

	using namespace Bind;
	namespace dx = DirectX;
	auto model = Cube::MakeIndependentTextured();
	model.Transform(dx::XMMatrixScaling(size, size, size));
	model.SetNormalsIndependentFlat();
	model.ComputeTangentBiTtngent();
	const auto geometryTag = "$cube." + std::to_string(size);

	AddBind(Sampler::Resolve(gfx, 0u, Sampler::SamplerState::SSAnistropicWrap));

	AddBind(VertexBuffer::Resolve(gfx, geometryTag, model.vertices));
	AddBind(IndexBuffer::Resolve(gfx, geometryTag, model.indices));

	AddBind(Texture::Resolve(gfx, "Data\\Images\\spnza_bricks_a_diff.png"));
	AddBind(Texture::Resolve(gfx, "Data\\Images\\spnza_bricks_a_ddn.png", 2u));

	auto pvs = VertexShader::Resolve(gfx, "PhongVSNormalMap.cso", "PhongVSNormalMap.hlsl");
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(PixelShader::Resolve(gfx, "PhongPSNormalMap.cso", "PhongPSNormalMap.hlsl"));

	AddBind(PixelConstantBuffer<PSMaterialConstant>::Resolve(gfx, pmc, 2u));

	AddBind(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_shared<TransformPixelCbuf>(gfx, *this, 0u, 0u));

	AddBind(std::make_shared<Blender>(gfx, true, 1.0f));

	AddBind(Rasterizer::Resolve(gfx, Rasterizer::RasterizerState::RSCull));
}



DirectX::XMMATRIX TestCube::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(rollPitchYaw.x, rollPitchYaw.y, rollPitchYaw.z) *
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z)*
		DirectX::XMMatrixScaling(scale.x,scale.y,scale.z);
}

void TestCube::SpawnControlWindow(Graphics& gfx) noexcept
{
	if (ImGui::Begin("Cube"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &pos.x, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Y", &pos.y, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Z", &pos.z, -80.0f, 80.0f, "%.1f");
		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &rollPitchYaw.x, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &rollPitchYaw.y, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &rollPitchYaw.z, -180.0f, 180.0f);

		bool changed0 = ImGui::SliderFloat("Spec. Int.", &pmc.specularIntensity, 0.0f, 1.0f);
		bool changed1 = ImGui::SliderFloat("Spec. Power", &pmc.specularPower, 0.0f, 100.0f);
		bool checkState = pmc.normalMappingEnabled == TRUE;
		bool changed2 = ImGui::Checkbox("Enable Normal Map", &checkState);
		pmc.normalMappingEnabled = checkState ? TRUE : FALSE;
		if (changed0 || changed1 || changed2)
		{
			QueryBindable<Bind::PixelConstantBuffer<PSMaterialConstant>>()->Update(gfx, pmc);
		}	
		ImGui::Text("Shading");
		auto pBlender = QueryBindable<Bind::Blender>();
		float factor = pBlender->GetFactor();
		ImGui::SliderFloat("Translucency", &factor, 0.0f, 1.0f);
		pBlender->SetFactor(factor);
	}
	ImGui::End();
}

void TestCube::CreateBoundingBox()
{
	float radius = scale.x / 2.0f;
	DirectX::XMFLOAT3 minPos = { pos.x - radius,pos.y - radius,pos.z - radius };
	DirectX::XMFLOAT3 maxPos = { pos.x + radius,pos.y + radius,pos.z + radius };
	DirectX::BoundingBox::CreateFromPoints(boundingBox, DirectX::XMLoadFloat3(&minPos), DirectX::XMLoadFloat3(&maxPos));
}

DirectX::BoundingBox TestCube::GetLocalBoundingBox()  noexcept
{
	// TODO: insert return statement here
	return boundingBox;
}
