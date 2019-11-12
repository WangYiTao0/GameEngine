#include "GridTerrain.h"
#include "BindableCommon.h"
#include "GraphicsThrowMacros.h"
#include "Grid.h"

GridTerrain::GridTerrain(Graphics& gfx,float width, float depth,
	unsigned int m, unsigned int n)
{
	using namespace Bind;

	std::string shaderfolder = "";
#pragma region DetermineShaderPath
	if (IsDebuggerPresent() == TRUE)
	{
#ifdef _DEBUG //Debug Mode
#ifdef _WIN64 //x64
		shaderfolder = "..\\x64\\Debug\\";
#else  //x86 (Win32)
		shaderfolder = "..\\Debug\\";
#endif
#else //Release Mode
#ifdef _WIN64 //x64
		shaderfolder = "..\\x64\\Release\\";
#else  //x86 (Win32)
		shaderfolder = "..\\Release\\";
#endif
#endif
	}
	using Element = Dvtx::VertexLayout::ElementType;

	auto layout = Dvtx::VertexLayout{}
		.Append(Element::Position3D)
		.Append(Element::Normal)
		.Append(Element::Tangent)
		.Append(Element::Texture2D);
	auto model = Grid::MakeIndependent(layout,width,depth,m,n);
	const auto geometryTag = "$Grid." + std::to_string(width);
	model.SetNormalsIndependentFlat();

	AddBind(VertexBuffer::Resolve(gfx, geometryTag, model.vertices));
	AddBind(IndexBuffer::Resolve(gfx, geometryTag, model.indices));

	auto pvs = VertexShader::Resolve(gfx, shaderfolder + "PhongVS.cso");
	//auto pvsbc = static_cast<VertexShader&>(*pvs).GetBytecode();
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(PixelShader::Resolve(gfx, shaderfolder + "PhongPS.cso"));

	struct PSColorConstant
	{
		float specularIntensity;
		float specularPower;
		float padding[2];
	} phongConnst;

	AddBind(PixelConstantBuffer<PSColorConstant>::Resolve(gfx, phongConnst, 4u));

	AddBind(InputLayout::Resolve(gfx,
		model.vertices.GetLayout(), pvsbc));

	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));


	AddBind(std::make_shared<TransformCbuf>(gfx, *this));
}

void GridTerrain::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
	this->pos = pos;
}

DirectX::XMMATRIX GridTerrain::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}
