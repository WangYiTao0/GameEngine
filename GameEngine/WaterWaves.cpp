#include "WaterWaves.h"
#include "BindableCommon.h"
#include "Grid.h"
#include "StringHelper.h"
#include "TransformPixelCbuf.h"


WaterWaves::WaterWaves(Graphics& gfx, float width, float depth, unsigned int m, unsigned int n, float gridSize)
{
	using namespace Bind;
	 
	using Element = Dvtx::VertexLayout::ElementType;
	auto layout = Dvtx::VertexLayout{}
		.Append(Element::Position3D)
		.Append(Element::Float4Color);

	auto model = Grid::MakeIndependentColor(layout, width, depth, m, n);
	const auto geometryTag = "$WaterWaves" + std::to_string(width);

	//model.ComputeTangentBiTtngent();

	AddBind(Sampler::Resolve(gfx, 0u, Sampler::SamplerState::SSAnistropicWrap));

	AddBind(VertexBuffer::Resolve(gfx, geometryTag, model.vertices));
	AddBind(IndexBuffer::Resolve(gfx, geometryTag, model.indices));

	AddBind(Texture::Resolve(gfx, "Data\\Images\\water1.dds"));
	//AddBind(Texture::Resolve(gfx, "Data\\Images\\tile_nmap.dds", 2u));

	auto pvs = VertexShader::Resolve(gfx,  "WaterWavesVS.cso");

	//auto pvsbc = static_cast<VertexShader&>(*pvs).GetBytecode();
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(PixelShader::Resolve(gfx,  "WaterWavesPS.cso"));

	//AddBind(PixelConstantBuffer<PSMaterialConstant>::Resolve(gfx, pmc, 1u));

	AddBind(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_shared<Blender>(gfx, true, 0.5f));

	AddBind(Rasterizer::Resolve(gfx, Rasterizer::RasterizerState::RSNoCull));

	AddBind(std::make_shared<TransformCbuf>(gfx, *this));
	//AddBind(std::make_shared<TransformPixelCbuf>(gfx, *this, 0u, 2u));
}


void WaterWaves::Update(float dt)
{
	//DirectX::(x,z,dt)
}



DirectX::XMMATRIX WaterWaves::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(rollPitchYaw.x, rollPitchYaw.y, rollPitchYaw.z) *
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z) *
		DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
}

