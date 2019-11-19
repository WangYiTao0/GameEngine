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
		.Append(Element::Tangent)
		.Append(Element::Bitangent)
		.Append(Element::Texture2D);

	auto model = Grid::MakeIndependent(layout, width, depth, m, n, gridSize);
	const auto geometryTag = "$Grid." + std::to_string(width);
	//model.SetNormalsIndependentFlat();
	model.ComputeTangentBiTtngent();

	for (size_t i = 0; i < model.vertices.Size(); i ++)
	{
		auto fpos = model.vertices[i].Attr<Element::Position3D>();
		fpos.y = GetHillsHeight(fpos.x, fpos.z);
		auto pos = XMLoadFloat3(&fpos);

		XMStoreFloat3(&model.vertices[i].Attr<Element::Position3D>(), pos);
	}

	AddBind(Sampler::Resolve(gfx,0u,Sampler::SamplerState::SSAnistropicWrap));

	AddBind(VertexBuffer::Resolve(gfx, geometryTag, model.vertices));
	AddBind(IndexBuffer::Resolve(gfx, geometryTag, model.indices));

	AddBind(Texture::Resolve(gfx, "Data\\Images\\grass.dds"));
	//AddBind(Texture::Resolve(gfx, "Data\\Images\\tile_nmap.dds", 2u));

	auto pvs = VertexShader::Resolve(gfx, shaderfolder + "PhongVSNormalMap.cso");
	
	//auto pvsbc = static_cast<VertexShader&>(*pvs).GetBytecode();
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(PixelShader::Resolve(gfx, shaderfolder + "PhongPSNormalMap.cso"));


	AddBind(PixelConstantBuffer<PSMaterialConstant>::Resolve(gfx, pmc, 1u));

	AddBind(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_shared<Blender>(gfx, false, 0.5f));

	AddBind(Rasterizer::Resolve(gfx, false, true));

	//AddBind(std::make_shared<TransformCbuf>(gfx, *this));
	AddBind(std::make_shared<TransformPixelCbuf>(gfx, *this, 0u, 2u));
}

void GridTerrain::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
	this->pos = pos;
}

DirectX::XMMATRIX GridTerrain::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}

void GridTerrain::SpawnControlWindow(Graphics& gfx) noexcept
{
	if (ImGui::Begin("Grid"))
	{
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
