#include "Tex2D.h"
#include "Plane.h"
#include "BindableCommon.h"
#include "StringHelper.h"

Tex2D::Tex2D(Graphics& gfx, float width, float height, std::string filePath)
{
	using namespace Bind;
	
	auto tex2D = Plane::Make2DTextured();

	const auto geometryTag = "$plane." + std::to_string(width);

	AddBind(VertexBuffer::Resolve(gfx, geometryTag, tex2D.vertices));
	AddBind(IndexBuffer::Resolve(gfx, geometryTag, tex2D.indices));
	AddBind(Sampler::Resolve(gfx, 0u, Sampler::SamplerState::SSLinearWrap));
	AddBind(Texture::Resolve(gfx, filePath));

	auto pvs = VertexShader::Resolve(gfx, "VS_2D.cso", "VS_2D.hlsl");
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(PixelShader::Resolve(gfx, "PS_2D.cso", "PS_2D.hlsl"));

	AddBind(InputLayout::Resolve(gfx, tex2D.vertices.GetLayout(), pvsbc));
	AddBind(std::make_shared<TransformCbuf>(gfx, *this, 0u));

	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	AddBind(std::make_shared<Blender>(gfx, false, 1.0f));

	AddBind(Rasterizer::Resolve(gfx, Rasterizer::RasterizerState::RSDepthDisable));


	SetPos({ 0.0f, 0.0f, 0.0f });
	SetRotation({ 0.0f, 0.0f, 0.0f});
	SetScale({ width, height, 1.0f });
}

DirectX::XMMATRIX Tex2D::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(rollPitchYaw.x, rollPitchYaw.y, rollPitchYaw.z) *
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z) *
		DirectX::XMMatrixScaling(scale.x, scale.y,1.0f);
}
