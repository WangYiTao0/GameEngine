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

	AddBind(Sampler::Resolve(gfx, 0u, Sampler::SamplerState::SSAnistropicWrap));


	AddBind(VertexBuffer::Resolve(gfx, geometryTag, model.vertices));
	AddBind(IndexBuffer::Resolve(gfx, geometryTag, model.indices));

	AddBind(std::make_shared<Texture>(gfx, filePaths, 0u));

	auto pvs = VertexShader::Resolve(gfx, "Sky_VS.cso", "Sky_VS.hlsl");
	
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(PixelShader::Resolve(gfx, "Sky_PS.cso", "Sky_PS.hlsl"));

	AddBind(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_shared<TransformCbuf>(gfx, *this));

	AddBind(DepthStencil::Resolve(gfx, DepthStencil::DepthStencilState::DSSLessEqual));

	AddBind(Blender::Resolve(gfx, false));

	AddBind(Rasterizer::Resolve(gfx, Rasterizer::RasterizerState::RSNoCull));
}

DirectX::XMMATRIX SkyRender::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

}
