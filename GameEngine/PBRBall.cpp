#include "PBRBall.h"
#include "BindableCommon.h"
#include "GraphicsThrowMacros.h"
#include "Sphere.h"
#include "Cube.h"

PBRBall::PBRBall(Graphics& gfx, float radius)
{
	using namespace Bind;
	namespace dx = DirectX;


	using Element = Dvtx::VertexLayout::ElementType;

	auto layout = Dvtx::VertexLayout{}
		.Append(Element::Position3D)
		.Append(Element::Normal)
		.Append(Element::Tangent)
		.Append(Element::Bitangent)
		.Append(Element::Texture2D);

	auto model = Sphere::MakeSphereTexture(layout,48,92);
	//auto model = Cube::MakeIndependentTextured(layout);
	model.Transform(dx::XMMatrixScaling(radius, radius, radius));
	model.SetNormalsIndependentFlat();
	model.ComputeSphereUV();
	model.ComputeTangentBiTtngent();


	const auto geometryTag = "$sphere." + std::to_string(radius);
	AddBind(VertexBuffer::Resolve(gfx, geometryTag, model.vertices));
	AddBind(IndexBuffer::Resolve(gfx, geometryTag, model.indices));

	AddBind(Texture::Resolve(gfx, "Data\\Images\\PBR\\rusted_iron\\albedo.png",0u));
	AddBind(Texture::Resolve(gfx, "Data\\Images\\PBR\\rusted_iron\\normal.png",1u));
	AddBind(Texture::Resolve(gfx, "Data\\Images\\PBR\\rusted_iron\\metallic.png",2u));
	AddBind(Texture::Resolve(gfx, "Data\\Images\\PBR\\rusted_iron\\roughness.png",3u));
	AddBind(Texture::Resolve(gfx, "Data\\Images\\PBR\\rusted_iron\\ao.png",4u));

	auto pvs = VertexShader::Resolve(gfx, "ForWard_BRDF_VS");
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(PixelShader::Resolve(gfx, "ForWard_BRDF_PS"));

	AddBind(Sampler::Resolve(gfx,0u, Sampler::SamplerState::SSLinearWrap));

	AddBind(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_shared<TransformCbuf>(gfx, *this));

	AddBind(Blender::Resolve(gfx, false));

	AddBind(Rasterizer::Resolve(gfx, Rasterizer::Mode::RSCullBack));
	AddBind(std::make_shared<DepthStencil>(gfx, DepthStencil::Mode::DSSOff));
}

DirectX::XMMATRIX PBRBall::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}
