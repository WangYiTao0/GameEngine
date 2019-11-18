#include "WaterWaves.h"
#include "BindableCommon.h"
#include "Grid.h"
#include "StringHelper.h"
#include "TransformPixelCbuf.h"


WaterWaves::WaterWaves(Graphics& gfx, float width, float depth, unsigned int m, unsigned int n, float gridSize)
{
	using namespace Bind;
	std::string shaderfolder = StringHelper::GetShaderRootPath();
	using Element = Dvtx::VertexLayout::ElementType;
	auto layout = Dvtx::VertexLayout{}
		.Append(Element::Position3D)
		.Append(Element::Normal)
		.Append(Element::Tangent)
		.Append(Element::Bitangent)
		.Append(Element::Texture2D);

	auto model = Grid::MakeIndependent(layout, width, depth, m, n, gridSize);
	const auto geometryTag = "$WaterWaves" + std::to_string(width);

	model.ComputeTangentBiTtngent();

	AddBind(Sampler::Resolve(gfx));

	AddBind(VertexBuffer::Resolve(gfx, geometryTag, model.vertices));
	AddBind(IndexBuffer::Resolve(gfx, geometryTag, model.indices));

	AddBind(Texture::Resolve(gfx, "Data\\Images\\water1.dds"));
	//AddBind(Texture::Resolve(gfx, "Data\\Images\\tile_nmap.dds", 2u));

	auto pvs = VertexShader::Resolve(gfx, shaderfolder + "PhongVSNormalMap.cso");

	//auto pvsbc = static_cast<VertexShader&>(*pvs).GetBytecode();
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(PixelShader::Resolve(gfx, shaderfolder + "PhongPSNormalMapObject.cso"));

	//AddBind(PixelConstantBuffer<PSMaterialConstant>::Resolve(gfx, pmc, 1u));

	AddBind(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_shared<Blender>(gfx, true, 0.5f));

	AddBind(Rasterizer::Resolve(gfx, true));

	AddBind(std::make_shared<TransformCbuf>(gfx, *this));
	AddBind(std::make_shared<TransformPixelCbuf>(gfx, *this, 0u, 2u));
}


void WaterWaves::Update(float dt)
{
	//DirectX::(x,z,dt)
}

void WaterWaves::Disturb(UINT i, UINT j, float magnitude)
{

}
