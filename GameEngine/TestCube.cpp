#include "TestCube.h"
#include "Cube.h"
#include "BindableCommon.h"
#include "TransformPixelCbuf.h"
#include "imgui/imgui.h"
#include "StringHelper.h"

TestCube::TestCube(Graphics& gfx, float size)
{
	this->scale = { size,size,size };

	using namespace Bind;
	namespace dx = DirectX;
	auto model = Cube::MakeIndependentTextured();
	model.Transform(dx::XMMatrixScaling(size, size, size));
	model.SetNormalsIndependentFlat();
	//model.ComputeTangentBiTtngent();
	const auto geometryTag = "$cube." + std::to_string(size);

	AddBind(Sampler::Resolve(gfx, 0u, Sampler::SamplerState::SSAnistropicWrap));
	AddBind(VertexBuffer::Resolve(gfx, geometryTag, model.vertices));
	AddBind(IndexBuffer::Resolve(gfx, geometryTag, model.indices));
	AddBind(Texture::Resolve(gfx, "Data\\Images\\spnza_bricks_a_diff.png"));
	//AddBind(Texture::Resolve(gfx, "Data\\Images\\spnza_bricks_a_ddn.png", 2u));
	auto pvs = VertexShader::Resolve(gfx, "PhongVS");
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));
	AddBind(PixelShader::Resolve(gfx, "PhongPS"));
	AddBind(PixelConstantBuffer<Material>::Resolve(gfx, pmc, 2u));
	AddBind(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));
	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	auto tfbvp =  std::make_shared<TransformVertexAndPixelCbuf>(gfx, *this, 0u, 0u);
	AddBind(tfbvp);
	AddBind(std::make_shared<Blender>(gfx, true, 1.0f));
	AddBind(Rasterizer::Resolve(gfx, Rasterizer::Mode::RSCull));
	AddBind(DepthStencil::Resolve(gfx, DepthStencil::Mode::DSSWrite));


	outlineEffect.push_back(VertexBuffer::Resolve(gfx, geometryTag, model.vertices));
	outlineEffect.push_back(IndexBuffer::Resolve(gfx, geometryTag, model.indices));
	pvs = VertexShader::Resolve(gfx, "SolidVS");
	pvsbc = pvs->GetBytecode();
	outlineEffect.push_back(std::move(pvs));
	outlineEffect.push_back(PixelShader::Resolve(gfx, "SolidPS"));
	struct SolidColorBuffer
	{
		DirectX::XMFLOAT4 color = { 1.0f,0.4f,0.4f,1.0f };
	} scb;
	outlineEffect.push_back(PixelConstantBuffer<SolidColorBuffer>::Resolve(gfx, scb, 3u));
	outlineEffect.push_back(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));
	outlineEffect.push_back(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	outlineEffect.push_back(std::move(tfbvp));
	outlineEffect.push_back(std::make_shared<DepthStencil>(gfx, DepthStencil::Mode::DSSMask));


	depth.push_back(VertexBuffer::Resolve(gfx, geometryTag, model.vertices));
	depth.push_back(IndexBuffer::Resolve(gfx, geometryTag, model.indices));
	pvs = VertexShader::Resolve(gfx, "SimpleDepth_VS");
	pvsbc = pvs->GetBytecode();
	depth.push_back(std::move(pvs));
	depth.push_back(PixelShader::Resolve(gfx, "SimpleDepth_PS"));
	depth.push_back(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));
	depth.push_back(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	depth.push_back(std::make_shared<TransformCbuf>(gfx, *this));
	depth.push_back(std::make_shared<DepthStencil>(gfx, DepthStencil::Mode::DSSOff));



	//depth.push_back(std::make_shared<DepthStencil>(gfx, DepthStencil::Mode::DSSMask));
}



DirectX::XMMATRIX TestCube::GetTransformXM() const noexcept
{
	auto xf = DirectX::XMMatrixRotationRollPitchYaw(rollPitchYaw.x, rollPitchYaw.y, rollPitchYaw.z) *
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z)*
		DirectX::XMMatrixScaling(scale.x,scale.y,scale.z);

	if (outlining)
	{
		xf = DirectX::XMMatrixScaling(1.03f, 1.03f, 1.03f) * xf;
	}
	return xf;
}


void TestCube::DrawOutline(Graphics& gfx) noxnd
{
	outlining = true;
	for (auto& b : outlineEffect)
	{
		b->Bind(gfx);
	}
	gfx.DrawIndexed(QueryBindable<Bind::IndexBuffer>()->GetCount());
	outlining = false;
}

void TestCube::DrawDepth(Graphics& gfx)noexcept
{
	for (auto& b : depth)
	{
		b->Bind(gfx);
	}
	gfx.DrawIndexed(QueryBindable<Bind::IndexBuffer>()->GetCount());
}

void TestCube::SpawnControlWindow(Graphics& gfx, const char* name) noexcept
{
	if (ImGui::Begin(name))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &pos.x, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Y", &pos.y, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Z", &pos.z, -80.0f, 80.0f, "%.1f");
		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &rollPitchYaw.x, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &rollPitchYaw.y, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &rollPitchYaw.z, -180.0f, 180.0f);

		ImGui::Text("Material");
		bool changed0 = ImGui::SliderFloat("Spec. shinness.", &pmc.shinness, 0.0f, 100.0f);
		bool changed1 = ImGui::ColorEdit3("diff.", &pmc.diff.x);
		bool changed2 = ImGui::ColorEdit3("Spec", &pmc.spec.x);
		if (changed0 || changed1 || changed2)
		{
			QueryBindable<Bind::PixelConstantBuffer<Material>>()->Update(gfx, pmc);
		}

		ImGui::Text("Shading");
		auto pBlender = QueryBindable<Bind::Blender>();
		float factor = pBlender->GetFactor();
		ImGui::SliderFloat("Translucency", &factor, 0.0f, 1.0f);
		pBlender->SetFactor(factor);
	}
	ImGui::End();
}

void TestCube::CreateBoundingBox()
{
	float radius = scale.x / 2.0f;
	DirectX::XMFLOAT3 minPos = { pos.x - radius,pos.y - radius,pos.z - radius };
	DirectX::XMFLOAT3 maxPos = { pos.x + radius,pos.y + radius,pos.z + radius };
	DirectX::BoundingBox::CreateFromPoints(boundingBox, DirectX::XMLoadFloat3(&minPos), DirectX::XMLoadFloat3(&maxPos));
}

DirectX::BoundingBox TestCube::GetLocalBoundingBox()  noexcept
{
	// TODO: insert return statement here
	return boundingBox;
}
