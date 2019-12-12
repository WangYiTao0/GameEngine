//#include "Shadow.h"
//#include "BindableCommon.h"
//
//Shadow::Shadow(Graphics& gfx, std::string VS_name, std::string PS_name)
//{
//	using namespace Bind;
//	using namespace DirectX;
//
//	using Element = Dvtx::VertexLayout::ElementType;
//
//	auto layout = Dvtx::VertexLayout{}
//		.Append(Element::Position3D)
//		.Append(Element::Normal)
//		.Append(Element::Texture2D);
//	AddBind(Sampler::Resolve(gfx, 0u, Sampler::SamplerState::SSLinearWrap));
//	AddBind(Sampler::Resolve(gfx, 1u, Sampler::SamplerState::SSLinearClamp));
//	AddBind(VertexBuffer::Resolve(gfx, geometryTag, model.vertices));
//	AddBind(IndexBuffer::Resolve(gfx, geometryTag, model.indices));
//
//
//	auto pvs = VertexShader::Resolve(gfx, "VS_2D");
//	auto pvsbc = pvs->GetBytecode();
//	AddBind(std::move(pvs));
//
//	AddBind(PixelShader::Resolve(gfx, "PS_2D"));
//
//}
//
//DirectX::XMMATRIX Shadow::GetTransformXM() const noexcept
//{
//	return DirectX::XMMATRIX();
//}
