#include "WaterWave.h"
#include "BindableCommon.h"
#include "MathHelper.h"

IndexedTriangleList WaterWave::wave;

WaterWave::WaterWave(Graphics& gfx)
{
	using namespace Bind;
	using namespace DirectX;

	mWaves.Init(200, 200, 0.8f, 0.03f, 3.25f, 0.4f);
	using Element = Dvtx::VertexLayout::ElementType;
	auto layout = Dvtx::VertexLayout{}
	.Append(Element::Position3D);
	//without 0
	Dvtx::VertexBuffer vb(std::move(layout), mWaves.VertexCount());
	

	AddBind(Sampler::Resolve(gfx, 0u, Sampler::SamplerState::SSAnistropicWrap));
}

void WaterWave::Update(float dt)
{

	using namespace Bind;
	using namespace DirectX;

	using Element = Dvtx::VertexLayout::ElementType;
	//
	// Every quarter second, generate a random wave.
	//
	float last = dt;
	
	if ((dt - last) >= 0.25f)
	{
		last += 0.25f;

		DWORD i = 5 + rand() % 190;
		DWORD j = 5 + rand() % 190;

		float r = MathHelper::RandT(1.0f, 2.0f);

		mWaves.Disturb(i, j, r);
	}
	
	for (UINT i = 0; i < mWaves.VertexCount(); ++i)
	{
		wave.vertices.EmplaceBack(mWaves[i]);
		wave.indices.push_back(3 * mWaves.TriangleCount());
	}

	//const auto geometryTag = "$WaterWave.";
	//
	//AddBind(VertexBuffer::Resolve(gfx, geometryTag, wave.vertices));
	//AddBind(IndexBuffer::Resolve(gfx, geometryTag, wave.indices));

	mWaves.Update(dt);
}

DirectX::XMMATRIX WaterWave::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(rollPitchYaw.x, rollPitchYaw.y, rollPitchYaw.z) *
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z) *
		DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
}
