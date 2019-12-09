#include "SkyRender.h"
#include "BindableCommon.h"
#include "GraphicsThrowMacros.h"
#include "StringHelper.h"
#include "Sphere.h"

SkyRender::SkyRender(Graphics& gfx, std::vector<std::string>& filePaths, float skySphereRadius)
{
	using namespace Bind;
	
	auto model = Sphere::Make();
	model.Transform(DirectX::XMMatrixScaling(skySphereRadius, skySphereRadius, skySphereRadius));
	const auto geometryTag = "&skySphere." + std::to_string(skySphereRadius);

	AddBind(Sampler::Resolve(gfx, 0u, Sampler::SamplerState::SSLinearWrap));


	AddBind(VertexBuffer::Resolve(gfx, geometryTag, model.vertices));
	AddBind(IndexBuffer::Resolve(gfx, geometryTag, model.indices));

	AddBind(std::make_shared<Texture>(gfx, filePaths, 4u));

	auto pvs = VertexShader::Resolve(gfx, "Sky_VS");
	
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(PixelShader::Resolve(gfx, "Sky_PS"));

	AddBind(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_shared<TransformCbuf>(gfx, *this));

	AddBind(Rasterizer::Resolve(gfx, Rasterizer::RasterizerState::RSNoCull));

	AddBind(DepthStencil::Resolve(gfx, DepthStencil::DepthStencilState::DSSLessEqual));

	AddBind(Blender::Resolve(gfx, false));
}

DirectX::XMMATRIX SkyRender::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

}
