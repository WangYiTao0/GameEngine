#include "Tex2D.h"
#include "Plane.h"
#include "BindableCommon.h"
#include "StringHelper.h"


IndexedTriangleList Tex2D::tex2D ;

Tex2D::Tex2D(Graphics& gfx, float screenWidth, float screenHeight, float texWidth, float texHeight, std::string filePath)
{
	using namespace Bind;
	
    tex2D = Plane::Make2DTextured();

	const auto geometryTag = "$plane." + std::to_string(texWidth);

	AddBind(VertexBuffer::Resolve(gfx, geometryTag, tex2D.vertices));
	AddBind(IndexBuffer::Resolve(gfx, geometryTag, tex2D.indices));
	AddBind(Sampler::Resolve(gfx, 0u, Sampler::SamplerState::SSLinearWrap));
	AddBind(Texture::Resolve(gfx, filePath));

	auto pvs = VertexShader::Resolve(gfx, "VS_2D");
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(PixelShader::Resolve(gfx, "PS_2D"));

	AddBind(InputLayout::Resolve(gfx, tex2D.vertices.GetLayout(), pvsbc));
	AddBind(std::make_shared<TransformCbuf>(gfx, *this, 0u));

	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	AddBind(std::make_shared<Blender>(gfx, false, 1.0f));

	AddBind(Rasterizer::Resolve(gfx, Rasterizer::Mode::RSDepthDisable));


	SetPos({ 0.0f, 0.0f, 0.0f });
	SetRotation({ 0.0f, 0.0f, 0.0f});
	SetScale({ texWidth, texHeight, 1.0f });
}

Tex2D::Tex2D(Graphics& gfx, float screenWidth, float screenHeight, 
	float texWidth, float texHeight, std::string VS_Name, std::string PS_Name, ID3D11ShaderResourceView* pSv)
{
	using namespace Bind;

	auto tex2D = Plane::Make2DTextured();

	const auto geometryTag = "$planeDynamic." + std::to_string(texWidth);

	AddBind(VertexBuffer::Resolve(gfx, geometryTag, tex2D.vertices,D3D11_USAGE::D3D11_USAGE_DYNAMIC));
	AddBind(IndexBuffer::Resolve(gfx, geometryTag, tex2D.indices));
	AddBind(Sampler::Resolve(gfx, 0u, Sampler::SamplerState::SSLinearWrap));
	//AddBind(std::make_shared<Texture>(gfx, "null", 0u, pSv));
	AddBind(Texture::Resolve(gfx, "null", 0u, pSv));
	auto pvs = VertexShader::Resolve(gfx, VS_Name);
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(PixelShader::Resolve(gfx, PS_Name));

	AddBind(InputLayout::Resolve(gfx, tex2D.vertices.GetLayout(), pvsbc));
	AddBind(std::make_shared<TransformCbuf>(gfx, *this, 0u));

	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	AddBind(std::make_shared<Blender>(gfx, false, 1.0f));

	AddBind(Rasterizer::Resolve(gfx, Rasterizer::Mode::RSDepthDisable));


	SetPos({ 0.0f, 0.0f, 0.0f });
	SetRotation({ 0.0f, 0.0f, 0.0f });
	SetScale({ texWidth, texHeight, 1.0f });
}



void Tex2D::Update(Graphics& gfx)
{
	//Vertex* vt;
	////vt[0].pos =
	//QueryBindable<Bind::VertexBuffer>()->UpdateDynamicVertexBuffer(gfx, vt);
}

DirectX::XMMATRIX Tex2D::GetTransformXM() const noexcept
{
	return
		DirectX::XMMatrixScaling(scale.x, scale.y, 1.0f)
		* DirectX::XMMatrixRotationRollPitchYaw(rollPitchYaw.x, rollPitchYaw.y, rollPitchYaw.z)
		* DirectX::XMMatrixTranslation(pos.x + scale.x / 2.0f, pos.y + scale.y / 2.0f, pos.z);
}
