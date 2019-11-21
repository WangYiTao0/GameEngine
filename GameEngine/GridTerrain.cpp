#include "GridTerrain.h"
#include "StringHelper.h"
#include "BindableCommon.h"
#include "GraphicsThrowMacros.h"
#include "TransformPixelCbuf.h"
#include "Grid.h"
#include "imgui/imgui.h"

GridTerrain::GridTerrain(Graphics& gfx, float width , float depth ,
	unsigned int m , unsigned int n , float gridSize )
{
	using namespace Bind;
	using namespace DirectX;

	std::string shaderfolder = StringHelper::GetShaderRootPath();

	using Element = Dvtx::VertexLayout::ElementType;

	auto layout = Dvtx::VertexLayout{}
		.Append(Element::Position3D)
		.Append(Element::Normal)
		.Append(Element::Texture2D);

	auto model = Grid::MakeIndependent(layout, width, depth, m, n, gridSize);
	const auto geometryTag = "$Grid." + std::to_string(width);
	model.SetNormalsIndependentFlat();


	AddBind(Sampler::Resolve(gfx,0u,Sampler::SamplerState::SSAnistropicWrap;

	AddBind(VertexBuffer::Resolve(gfx, geometryTag, model.vertices));
	AddBind(IndexBuffer::Resolve(gfx, geometryTag, model.indices));

	//AddBind(Texture::Resolve(gfx, "Data\\Images\\tile.dds"));
	//AddBind(Texture::Resolve(gfx, "Data\\Images\\tile_nmap.dds", 2u));
	AddBind(Texture::Resolve(gfx, "Data\\Images\\brickwall.jpg"));
	//AddBind(Texture::Resolve(gfx, "Data\\Images\\brickwall_normal.jpg", 2u));

	auto pvs = VertexShader::Resolve(gfx, shaderfolder + "PhongVS.cso");
	
	//auto pvsbc = static_cast<VertexShader&>(*pvs).GetBytecode();
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(PixelShader::Resolve(gfx, shaderfolder + "PhongPS.cso"));

	AddBind(PixelConstantBuffer<PSMaterialConstant>::Resolve(gfx, pmc, 2u));

	AddBind(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_shared<Blender>(gfx, false, 0.5f));

	AddBind(Rasterizer::Resolve(gfx, Rasterizer::RasterizerState::RSCull));

	//AddBind(std::make_shared<TransformCbuf>(gfx, *this));
	AddBind(std::make_shared<TransformPixelCbuf>(gfx, *this, 0u, 0u));
}


DirectX::XMMATRIX GridTerrain::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}

void GridTerrain::SpawnControlWindow(Graphics& gfx) noexcept
{
	if (ImGui::Begin("Grid"))
	{
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
