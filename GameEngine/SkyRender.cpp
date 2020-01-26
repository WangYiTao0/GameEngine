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

	pVertices = VertexBuffer::Resolve(gfx, geometryTag, model.vertices);
	pIndices = IndexBuffer::Resolve(gfx, geometryTag, model.indices);
	pTopology = Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	{
		Technique Sky;
		{
			Step only(3);

			only.AddBindable(Sampler::Resolve(gfx, 0u, Sampler::SamplerState::SSLinearWrap));

			only.AddBindable(std::make_shared<Texture>(gfx, filePaths, 4u));

			auto pvs = VertexShader::Resolve(gfx, "Sky_VS");

			auto pvsbc = pvs->GetBytecode();
			only.AddBindable(std::move(pvs));

			only.AddBindable(PixelShader::Resolve(gfx, "Sky_PS"));

			only.AddBindable(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

			only.AddBindable(std::make_shared<TransformCbuf>(gfx));

			only.AddBindable(Rasterizer::Resolve(gfx, Rasterizer::Mode::RSNoCull));

			only.AddBindable(Blender::Resolve(gfx, false));
			
			Sky.AddStep(std::move(only));
		}
		AddTechnique(std::move(Sky));
	}
}

DirectX::XMMATRIX SkyRender::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

}
