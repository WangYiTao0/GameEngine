#include "SolidSphere.h"
#include "BindableCommon.h"
#include "GraphicsThrowMacros.h"
#include "Sphere.h"
//#include "StrH.h"


SolidSphere::SolidSphere(Graphics& gfx, float radius)
{
	using namespace Bind;
	namespace dx = DirectX;

	auto model = Sphere::Make();
	model.Transform(dx::XMMatrixScaling(radius, radius, radius));
	const auto geometryTag = "$sphere." + std::to_string(radius);
	pVertices = VertexBuffer::Resolve(gfx, geometryTag, model.vertices);
	pIndices = IndexBuffer::Resolve(gfx, geometryTag, model.indices);
	pTopology = Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	{
		Technique solid;
		Step only(0);

		auto pvs = VertexShader::Resolve(gfx, "SolidVS");
		auto pvsbc = pvs->GetBytecode();
		only.AddBindable(std::move(pvs));

		only.AddBindable(PixelShader::Resolve(gfx, "SolidPS"));

		struct PSColorConstant
		{
			dx::XMFLOAT3 color = { 1.0f,1.0f,1.0f };
			float padding;
		} colorConst;
		only.AddBindable(PixelConstantBuffer<PSColorConstant>::Resolve(gfx, colorConst, 3u));

		only.AddBindable(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

		only.AddBindable(std::make_shared<TransformCbuf>(gfx));

		only.AddBindable(Blender::Resolve(gfx, false));

		only.AddBindable(Rasterizer::Resolve(gfx, Rasterizer::Mode::RSCullBack));

		solid.AddStep(std::move(only));
		AddTechnique(std::move(solid));
	}

	//auto pvs = VertexShader::Resolve(gfx, "SolidVS");
	////auto pvsbc = static_cast<VertexShader&>(*pvs).GetBytecode();
	//auto pvsbc = pvs->GetBytecode();
	//AddBind(std::move(pvs));

	//AddBind(PixelShader::Resolve(gfx, "SolidPS"));

	//struct PSColorConstant
	//{
	//	dx::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };
	//} colorConst;

	//AddBind(PixelConstantBuffer<PSColorConstant>::Resolve(gfx, colorConst,3u));

	//AddBind(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

	//AddBind(std::make_shared<TransformCbuf>(gfx, *this));

	//AddBind(Blender::Resolve(gfx, false));

	//AddBind(Rasterizer::Resolve(gfx, Rasterizer::Mode::RSCullBack));
	////AddBind(std::make_shared<DepthStencil>(gfx, DepthStencil::Mode::DSSOff));
	//AddBind(DepthStencil::Resolve(gfx, DepthStencil::Mode::DSSWrite));
}

DirectX::XMMATRIX SolidSphere::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}
