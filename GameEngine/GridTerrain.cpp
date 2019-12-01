#include "GridTerrain.h"
#include "StringHelper.h"
#include "MathHelper.h"
#include "BindableCommon.h"
#include "GraphicsThrowMacros.h"
#include "TransformPixelCbuf.h"
#include "Grid.h"
#include "imgui/imgui.h"

IndexedTriangleList GridTerrain::model;

GridTerrain::GridTerrain(Graphics& gfx, float width , float depth ,
	unsigned int m , unsigned int n , float gridSize )
{
	using namespace Bind;
	using namespace DirectX;

	using Element = Dvtx::VertexLayout::ElementType;

	auto layout = Dvtx::VertexLayout{}
		.Append(Element::Position3D)
		.Append(Element::Normal)
		.Append(Element::Tangent)
		.Append(Element::Bitangent)
		.Append(Element::Texture2D);

    model = Grid::MakeGrid(layout, width, depth, m, n, gridSize);
	model.SetNormalsIndependentFlat();
	model.ComputeTangentBiTtngent();

	const auto geometryTag = "$Grid." + std::to_string(width);
	AddBind(Sampler::Resolve(gfx,0u,Sampler::SamplerState::SSAnistropicWrap));
	AddBind(VertexBuffer::Resolve(gfx, geometryTag, model.vertices));
	AddBind(IndexBuffer::Resolve(gfx, geometryTag, model.indices));

	//AddBind(Texture::Resolve(gfx, "Data\\Images\\tile.dds"));
	//AddBind(Texture::Resolve(gfx, "Data\\Images\\tile_nmap.dds", 2u));
	AddBind(Texture::Resolve(gfx, "Data\\Images\\spnza_bricks_a_diff.png"));
	AddBind(Texture::Resolve(gfx, "Data\\Images\\spnza_bricks_a_ddn.png", 2u));

	auto pvs = VertexShader::Resolve(gfx, "PhongVSNormalMap.cso", "PhongVSNormalMap.hlsl");
	//auto pvs = VertexShader::Resolve(gfx, "WaterWavesVS.cso", "WaterWavesVS.hlsl");
	//auto pvsbc = static_cast<VertexShader&>(*pvs).GetBytecode();
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(PixelShader::Resolve(gfx, "PhongPSNormalMap.cso", "PhongPSNormalMap.hlsl"));
	//AddBind(PixelShader::Resolve(gfx, "WaterWavesPS.cso", "WaterWavesPS.hlsl"));

	AddBind(PixelConstantBuffer<PSMaterialConstant>::Resolve(gfx, pmc, 2u));


	AddBind(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_shared<Blender>(gfx, true, 1.0f));

	AddBind(Rasterizer::Resolve(gfx, Rasterizer::RasterizerState::RSWireframe));

	//AddBind(std::make_shared<TransformCbuf>(gfx, *this));
	AddBind(std::make_shared<TransformPixelCbuf>(gfx, *this, 0u, 0u));


}


DirectX::XMMATRIX GridTerrain::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(rollPitchYaw.x, rollPitchYaw.y, rollPitchYaw.z) *
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z) *
		DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
}

void GridTerrain::Update(Graphics& gfx,float dt)
{
}

void GridTerrain::SpawnControlWindow(Graphics& gfx) noexcept
{
	if (ImGui::Begin("Grid"))
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

float GridTerrain::GetHillsHeight(float x, float z) const
{
	return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z));
}
