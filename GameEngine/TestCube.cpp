#include "TestCube.h"
#include "Cube.h"
#include "BindableCommon.h"
#include "TransformPixelCbuf.h"
#include "imgui/imgui.h"
#include "StringHelper.h"
#include "DynamicConstant.h"
#include "TechniqueProbe.h"
#include "ConstantBuffersEx.h"


TestCube::TestCube(Graphics& gfx, float size)
{
	this->scale = { size,size,size };

	using namespace Bind;
	namespace dx = DirectX;

	using Element = Dvtx::VertexLayout::ElementType;

	auto layout = Dvtx::VertexLayout{}
		.Append(Element::Position3D)
		.Append(Element::Normal)
		.Append(Element::Texture2D);

	auto model = Cube::MakeIndependentTextured(layout);
	model.Transform(dx::XMMatrixScaling(size, size, size));
	model.SetNormalsIndependentFlat();
	//model.ComputeTangentBiTtngent();
	const auto geometryTag = "$cube." + std::to_string(size);

	pVertices = VertexBuffer::Resolve(gfx, geometryTag, model.vertices);
	pIndices = IndexBuffer::Resolve(gfx, geometryTag, model.indices);
	pTopology = Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	{
		Technique shade("Shade");
		{
			Step only(0);

			
			only.AddBindable(Sampler::Resolve(gfx, 0u, Sampler::SamplerState::SSAnistropicWrap));
			only.AddBindable(Sampler::Resolve(gfx, 1u, Sampler::SamplerState::SSLinearClamp));
			only.AddBindable(Texture::Resolve(gfx, "Data\\Images\\spnza_bricks_a_diff.png"));
			only.AddBindable(Texture::Resolve(gfx, "null", 3u, nullptr));

			auto pvs = VertexShader::Resolve(gfx, "PhongVS");
			auto pvsbc = pvs->GetBytecode();
			only.AddBindable(std::move(pvs));

			only.AddBindable(PixelShader::Resolve(gfx, "PhongPS"));

			Dcb::RawLayout lay;
			lay.Add<Dcb::Float>("specularIntensity");
			lay.Add<Dcb::Float>("specularPower");
			auto buf = Dcb::Buffer(std::move(lay));
			buf["specularIntensity"] = 0.1f;
			buf["specularPower"] = 20.0f;
			only.AddBindable(std::make_shared<Bind::CachingPixelConstantBufferEX>(gfx, buf, 2u));

			//only.AddBindable(PixelConstantBuffer<Material>::Resolve(gfx, pmc, 2u));

			only.AddBindable(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

			only.AddBindable(std::make_shared<TransformVertexAndPixelCbuf>(gfx, *this, 0u, 0u));

			shade.AddStep(std::move(only));
		}
		AddTechnique(std::move(shade));

	}
	//AddBind(Sampler::Resolve(gfx, 0u, Sampler::SamplerState::SSAnistropicWrap));
	//AddBind(Sampler::Resolve(gfx, 1u, Sampler::SamplerState::SSLinearClamp));

	//AddBind(Texture::Resolve(gfx, "Data\\Images\\spnza_bricks_a_diff.png"));
	////AddBind(Texture::Resolve(gfx, "Data\\Images\\spnza_bricks_a_ddn.png", 2u));
	//AddBind(Texture::Resolve(gfx,"null",3u,nullptr));
	//auto pvs = VertexShader::Resolve(gfx, "PhongVS");
	//auto pvsbc = pvs->GetBytecode();
	//AddBind(std::move(pvs));
	//AddBind(PixelShader::Resolve(gfx, "PhongPS"));
	//AddBind(PixelConstantBuffer<Material>::Resolve(gfx, pmc, 2u));
	//AddBind(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));
	//auto tfbvp =  std::make_shared<TransformVertexAndPixelCbuf>(gfx, *this, 0u, 0u);
	//AddBind(tfbvp);
	//AddBind(std::make_shared<Blender>(gfx, true, 1.0f));
	//AddBind(Rasterizer::Resolve(gfx, Rasterizer::Mode::RSCullBack));
	//AddBind(DepthStencil::Resolve(gfx, DepthStencil::Mode::DSSWrite));

	{
		Technique outline("Outline");
		{
			Step mask(1);

			auto pvs = VertexShader::Resolve(gfx, "SolidVS");
			auto pvsbc = pvs->GetBytecode();
			mask.AddBindable(std::move(pvs));

			// TODO: better sub-layout generation tech for future consideration maybe
			mask.AddBindable(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

			mask.AddBindable(std::make_shared<TransformCbuf>(gfx));

			// TODO: might need to specify rasterizer when doubled-sided models start being used

			outline.AddStep(std::move(mask));
		}
		{
			Step draw(2);

			auto pvs = VertexShader::Resolve(gfx, "SolidVS");
			auto pvsbc = pvs->GetBytecode();
			draw.AddBindable(std::move(pvs));

			// this can be pass-constant
			draw.AddBindable(PixelShader::Resolve(gfx, "SolidPS"));

			Dcb::RawLayout lay;
			lay.Add<Dcb::Float4>("color");
			auto buf = Dcb::Buffer(std::move(lay));
			buf["color"] = DirectX::XMFLOAT4{ 1.0f,0.4f,0.4f,1.0f };
			draw.AddBindable(std::make_shared<Bind::CachingPixelConstantBufferEX>(gfx, buf, 3u));


			// TODO: better sub-layout generation tech for future consideration maybe
			draw.AddBindable(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

			// quick and dirty... nicer solution maybe takes a lamba... we'll see :)
			class TransformCbufScaling : public TransformCbuf
			{
			public:
				TransformCbufScaling(Graphics& gfx, float scale = 1.04)
					:
					TransformCbuf(gfx),
					buf(MakeLayout())
				{
					buf["scale"] = scale;
				}
				void Accept(TechniqueProbe& probe) override
				{
					probe.VisitBuffer(buf);
				}
				void Bind(Graphics& gfx) noexcept override
				{
					const float scale = buf["scale"];
					const auto scaleMatrix = dx::XMMatrixScaling(scale, scale, scale);
					auto xf = GetTransforms(gfx);
					xf.world = xf.world * scaleMatrix;
					UpdateBindImpl(gfx, xf);
				}
			private:
				static Dcb::RawLayout MakeLayout()
				{
					Dcb::RawLayout layout;
					layout.Add<Dcb::Float>("scale");
					return layout;
				}
			private:
				Dcb::Buffer buf;
			};
			draw.AddBindable(std::make_shared<TransformCbufScaling>(gfx));

			// TODO: might need to specify rasterizer when doubled-sided models start being used

			outline.AddStep(std::move(draw));
		}
		AddTechnique(std::move(outline));
	}


	//pvs = VertexShader::Resolve(gfx, "SolidVS");
	//pvsbc = pvs->GetBytecode();
	//outlineEffect.push_back(std::move(pvs));
	//outlineEffect.push_back(PixelShader::Resolve(gfx, "SolidPS"));
	//struct SolidColorBuffer
	//{
	//	DirectX::XMFLOAT4 color = { 1.0f,0.4f,0.4f,1.0f };
	//} scb;
	//outlineEffect.push_back(PixelConstantBuffer<SolidColorBuffer>::Resolve(gfx, scb, 3u));
	//outlineEffect.push_back(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

	//outlineEffect.push_back(std::move(tfbvp));
	//outlineEffect.push_back(std::make_shared<DepthStencil>(gfx, DepthStencil::Mode::DSSMask));


	//depth.push_back(Rasterizer::Resolve(gfx, Rasterizer::Mode::RSCullFront));
	//depth.push_back(VertexBuffer::Resolve(gfx, geometryTag, model.vertices));
	//depth.push_back(IndexBuffer::Resolve(gfx, geometryTag, model.indices));
	//pvs = VertexShader::Resolve(gfx, "Depth_VS");
	//pvsbc = pvs->GetBytecode();
	//depth.push_back(std::move(pvs));
	//depth.push_back(PixelShader::Resolve(gfx, "Depth_PS"));
	//depth.push_back(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));
	//depth.push_back(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	//depth.push_back(std::make_shared<TransformCbuf>(gfx, *this));
	//depth.push_back(std::make_shared<DepthStencil>(gfx, DepthStencil::Mode::DSSDefault));

	//depth.push_back(Rasterizer::Resolve(gfx, Rasterizer::Mode::RSCullBack));
}



DirectX::XMMATRIX TestCube::GetTransformXM() const noexcept
{
	return  DirectX::XMMatrixRotationRollPitchYaw(rollPitchYaw.x, rollPitchYaw.y, rollPitchYaw.z) *
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z)*
		DirectX::XMMatrixScaling(scale.x,scale.y,scale.z);
}


void TestCube::DrawDepth(Graphics& gfx)noexcept
{
	//for (auto& b : depth)
	//{
	//	b->Bind(gfx);
	//}
	//gfx.DrawIndexed(QueryBindable<Bind::IndexBuffer>()->GetCount());
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

		class Probe : public TechniqueProbe
		{
		public:
			void OnSetTechnique() override
			{
				using namespace std::string_literals;
				ImGui::TextColored({ 0.4f,1.0f,0.6f,1.0f }, pTech->GetName().c_str());
				bool active = pTech->IsActive();
				ImGui::Checkbox(("Tech Active##"s + std::to_string(techIdx)).c_str(), &active);
				pTech->SetActiveState(active);
			}
			bool OnVisitBuffer(Dcb::Buffer& buf) override
			{
				namespace dx = DirectX;
				float dirty = false;
				const auto dcheck = [&dirty](bool changed) {dirty = dirty || changed; };
				auto tag = [tagScratch = std::string{}, tagString = "##" + std::to_string(bufIdx)]
				(const char* label) mutable
				{
					tagScratch = label + tagString;
					return tagScratch.c_str();
				};

				if (auto v = buf["scale"]; v.Exists())
				{
					dcheck(ImGui::SliderFloat(tag("Scale"), &v, 1.0f, 2.0f, "%.3f", 3.5f));
				}
				if (auto v = buf["color"]; v.Exists())
				{
					dcheck(ImGui::ColorPicker4(tag("Color"), reinterpret_cast<float*>(&static_cast<dx::XMFLOAT4&>(v))));
				}
				if (auto v = buf["specularIntensity"]; v.Exists())
				{
					dcheck(ImGui::SliderFloat(tag("Spec. Intens."), &v, 0.0f, 1.0f));
				}
				if (auto v = buf["specularPower"]; v.Exists())
				{
					dcheck(ImGui::SliderFloat(tag("Glossiness"), &v, 1.0f, 100.0f, "%.1f", 1.5f));
				}
				return dirty;
			}
		} probe;

		Accept(probe);
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
