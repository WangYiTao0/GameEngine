#include "SolidSphere.h"
#include "BindableCommon.h"
#include "GraphicsThrowMacros.h"
#include "Sphere.h"
#include "StringHelper.h"


SolidSphere::SolidSphere(Graphics& gfx, float radius)
{

	std::string shaderfolder = StringHelper::GetShaderRootPath();

	using namespace Bind;
	namespace dx = DirectX;

	auto model = Sphere::Make();
	model.Transform(dx::XMMatrixScaling(radius, radius, radius));
	const auto geometryTag = "$sphere." + std::to_string(radius);
	AddBind(VertexBuffer::Resolve(gfx,geometryTag,model.vertices));
	AddBind(IndexBuffer::Resolve(gfx,geometryTag,model.indices));

	auto pvs =VertexShader::Resolve(gfx, shaderfolder + "SolidVS.cso");
	//auto pvsbc = static_cast<VertexShader&>(*pvs).GetBytecode();
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(PixelShader::Resolve (gfx, shaderfolder + "SolidPS.cso"));

	struct PSColorConstant
	{
		dx::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };
	} colorConst;

	AddBind(PixelConstantBuffer<PSColorConstant>::Resolve(gfx, colorConst,1u));

	AddBind(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_shared<TransformCbuf>(gfx, *this));

	AddBind(Blender::Resolve(gfx, false));

	AddBind(Rasterizer::Resolve(gfx, Rasterizer::RasterizerState::RSCullClockWise));
}


void SolidSphere::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
	this->pos = pos;
}

DirectX::XMMATRIX SolidSphere::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}
