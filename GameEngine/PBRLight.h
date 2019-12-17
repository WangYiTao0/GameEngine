#pragma once
#include "Graphics.h"
#include "SolidSphere.h"
#include "ConstantBuffers.h"
#include "ConditionalNoexcept.h"
class PBRLight
{
public:
	PBRLight(Graphics& gfx);
	void Bind(Graphics& gfx);
private:
	struct PBRLightCB
	{
		DirectX::XMFLOAT4 position[4];
		DirectX::XMFLOAT4 color[4];
	};
private:
	PBRLightCB lightData;
	Bind::PixelConstantBuffer<PBRLightCB> cbuf;
};