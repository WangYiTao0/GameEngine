#pragma once
#pragma once
#include <array>
#include "BindableCommon.h"
#include "Graphics.h"
#include "Job.h"
#include "Pass.h"

class FrameCommander
{
public:
	FrameCommander() = default;

	void Accept(Job job, size_t target) noexcept
	{
		passes[target].Accept(job);
	}
	void Execute(Graphics& gfx) const noxnd
	{
		using namespace Bind;
		// normally this would be a loop with each pass defining it setup / etc.
		// and later on it would be a complex graph with parallel execution contingent
		// on input / output requirements

		// main phong lighting pass
		DepthStencil::Resolve(gfx, DepthStencil::Mode::DSSDefault)->Bind(gfx);
		passes[0].Execute(gfx);
		// outline masking pass
		DepthStencil::Resolve(gfx, DepthStencil::Mode::DSSWrite)->Bind(gfx);
		NullPixelShader::Resolve(gfx)->Bind(gfx);
		passes[1].Execute(gfx);
		// outline drawing pass
		DepthStencil::Resolve(gfx, DepthStencil::Mode::DSSMask)->Bind(gfx);
		struct SolidColorBuffer
		{
			DirectX::XMFLOAT4 color = { 0.0f,0.0f,0.0f,1.0f };
		} scb;
		PixelConstantBuffer<SolidColorBuffer>::Resolve(gfx, scb, 3u)->Bind(gfx);
		passes[2].Execute(gfx);

		DepthStencil::Resolve(gfx, DepthStencil::Mode::DSSLessEqual)->Bind(gfx);
		passes[3].Execute(gfx);
	}
	void Reset() noexcept
	{
		for (auto& p : passes)
		{
			p.Reset();
		}
	}
private:
	std::array<Pass, 5> passes;
};