#include "GridTerrain.h"
#include "StringHelper.h"
#include "MathHelper.h"
#include "BindableCommon.h"
#include "GraphicsThrowMacros.h"
#include "TransformPixelCbuf.h"
#include "Grid.h"
#include "imgui/imgui.h"
#include "App.h"

IndexedTriangleList GridTerrain::model ;

GridTerrain::GridTerrain(Graphics& gfx, float width , float depth ,
	unsigned int m , unsigned int n , float gridSize )
{
	//using namespace Bind;
	//using namespace DirectX;

	//using Element = Dvtx::VertexLayout::ElementType;

	//auto layout = Dvtx::VertexLayout{}
	//	.Append(Element::Position3D)
	//	.Append(Element::Normal)
	//	.Append(Element::Texture2D);

 //   model = Grid::MakeGrid(layout, width, depth, m, n, gridSize);
	////model.SetNormalsIndependentFlat();
	////model.ComputeTangentBiTtngent();

	//const auto geometryTag = "$Grid." + std::to_string(width);
	//AddBind(Sampler::Resolve(gfx,0u,Sampler::SamplerState::SSLinearWrap));
	//AddBind(Sampler::Resolve(gfx, 1u, Sampler::SamplerState::SSLinearClamp));
	//AddBind(VertexBuffer::Resolve(gfx, geometryTag, model.vertices));
	//AddBind(IndexBuffer::Resolve(gfx, geometryTag, model.indices));

	////AddBind(Texture::Resolve(gfx, "Data\\Images\\OpenArt\\156.jpg"));
	////AddBind(Texture::Resolve(gfx, "Data\\Images\\OpenArt\\156_norm.jpg", 2u));
	//AddBind(Texture::Resolve(gfx, "Data\\Images\\sponza_floor_a_diff.png"));
	////AddBind(Texture::Resolve(gfx, "Data\\Images\\sponza_floor_ddn.jpg", 2u));
	//AddBind(Texture::Resolve(gfx, "null", 3u, App::m_DepthRT->GetShaderResourceView()));

	//auto pvs = VertexShader::Resolve(gfx, "PhongVS");
	////auto pvs = VertexShader::Resolve(gfx, "WaterWavesVS.cso", "WaterWavesVS.hlsl");
	////auto pvsbc = static_cast<VertexShader&>(*pvs).GetBytecode(
	//auto pvsbc = pvs->GetBytecode();
	//AddBind(std::move(pvs));

	//AddBind(PixelShader::Resolve(gfx, "PhongShadow_PS"));
	////AddBind(PixelShader::Resolve(gfx, "WaterWavesPS.cso", "WaterWavesPS.hlsl"));

	//AddBind(PixelConstantBuffer<Material>::Resolve(gfx, pmc, 2u));

	//AddBind(InputLayout::Resolve(gfx, layout, pvsbc));

	//AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	//AddBind(std::make_shared<Blender>(gfx, true, 1.0f));

	//AddBind(Rasterizer::Resolve(gfx, Rasterizer::Mode::RSCullBack));

	////AddBind(std::make_shared<TransformCbuf>(gfx, *this));
	//AddBind(std::make_shared<TransformVertexAndPixelCbuf>(gfx, *this, 0u, 0u));

	//AddBind(std::make_shared<DepthStencil>(gfx, DepthStencil::Mode::DSSOff));


	//depths.push_back(Rasterizer::Resolve(gfx, Rasterizer::Mode::RSCullFront));
	//depths.push_back(VertexBuffer::Resolve(gfx, geometryTag, model.vertices));
	//depths.push_back(IndexBuffer::Resolve(gfx, geometryTag, model.indices));
	//pvs = VertexShader::Resolve(gfx, "Depth_VS");
	//pvsbc = pvs->GetBytecode();
	//depths.push_back(std::move(pvs));
	//depths.push_back(PixelShader::Resolve(gfx, "Depth_PS"));
	//depths.push_back(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));
	//depths.push_back(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	//depths.push_back(std::make_shared<TransformCbuf>(gfx, *this));
	//depths.push_back(std::make_shared<DepthStencil>(gfx, DepthStencil::Mode::DSSDefault));

	//depths.push_back(Rasterizer::Resolve(gfx, Rasterizer::Mode::RSCullBack));

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

void GridTerrain::DrawDepth(Graphics& gfx) noexcept
{
	//for (auto& b : depths)
	//{
	//	b->Bind(gfx);
	//}
	//gfx.DrawIndexed(QueryBindable<Bind::IndexBuffer>()->GetCount());
}

void GridTerrain::SpawnControlWindow(Graphics& gfx) noexcept
{
	//if (ImGui::Begin("Grid"))
	//{
	//	ImGui::Text("Position");
	//	ImGui::SliderFloat("X", &pos.x, -80.0f, 80.0f, "%.1f");
	//	ImGui::SliderFloat("Y", &pos.y, -80.0f, 80.0f, "%.1f");
	//	ImGui::SliderFloat("Z", &pos.z, -80.0f, 80.0f, "%.1f");
	//	ImGui::Text("Orientation");
	//	ImGui::SliderAngle("Roll", &rollPitchYaw.x, -180.0f, 180.0f);
	//	ImGui::SliderAngle("Pitch", &rollPitchYaw.y, -180.0f, 180.0f);
	//	ImGui::SliderAngle("Yaw", &rollPitchYaw.z, -180.0f, 180.0f);
	//	ImGui::Text("Material");
	//	bool changed0 = ImGui::SliderFloat("Specintensity.", &pmc.specularIntensity, 0.0f, 3.0f);
	//	bool changed1 = ImGui::SliderFloat("SpecPower.", &pmc.specularPower, 0.0f, 100.0f);
	//	if (changed0 || changed1)
	//	{
	//		QueryBindable<Bind::PixelConstantBuffer<Material>>()->Update(gfx, pmc);
	//	}
	//	ImGui::Text("Shading");
	//	auto pBlender = QueryBindable<Bind::Blender>();
	//	float factor = pBlender->GetFactor();
	//	ImGui::SliderFloat("Translucency", &factor, 0.0f, 1.0f);
	//	pBlender->SetFactor(factor);
	//}
	//ImGui::End();
}

float GridTerrain::GetHillsHeight(float x, float z) const
{
	return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z));
}
