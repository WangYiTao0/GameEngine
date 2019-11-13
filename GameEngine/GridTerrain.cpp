#include "GridTerrain.h"
#include "StringHelper.h"
#include "BindableCommon.h"
#include "GraphicsThrowMacros.h"
#include "TransformPixelCbuf.h"
#include "Grid.h"

GridTerrain::GridTerrain(Graphics& gfx,float width, float depth,
	unsigned int m, unsigned int n)
{
	using namespace Bind;

	std::string shaderfolder = StringHelper::GetShaderRootPath();

	using Element = Dvtx::VertexLayout::ElementType;

	auto layout = Dvtx::VertexLayout{}
		.Append(Element::Position3D)
		.Append(Element::Normal)
		.Append(Element::Tangent)
		.Append(Element::Texture2D);
	auto model = Grid::MakeIndependent(layout,width,depth,m,n);
	const auto geometryTag = "$Grid." + std::to_string(width);
	//model.SetNormalsIndependentFlat();

	AddBind(VertexBuffer::Resolve(gfx, geometryTag, model.vertices));
	AddBind(IndexBuffer::Resolve(gfx, geometryTag, model.indices));

	AddBind(Texture::Resolve(gfx, "Data\\Images\\brickwall.jpg"));
	AddBind(Texture::Resolve(gfx, "Data\\Images\\brickwall_normal.jpg", 1u));

	auto pvs = VertexShader::Resolve(gfx, shaderfolder + "PhongVS.cso");
	//auto pvsbc = static_cast<VertexShader&>(*pvs).GetBytecode();
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(PixelShader::Resolve(gfx, shaderfolder + "PhongPSNormalMapObject.cso"));

	//struct PSColorConstant
	//{
	//	float specularIntensity;
	//	float specularPower;
	//	float padding[2];
	//} phongConnst;

	AddBind(PixelConstantBuffer<PSMaterialConstant>::Resolve(gfx, pmc, 4u));

	AddBind(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));


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
