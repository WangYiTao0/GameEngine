#pragma once
#include "Graphics.h"
#include "ConstantBuffers.h"
#include "ConditionalNoexcept.h"

class DirectionLight
{
public:
	DirectionLight(Graphics& gfx);
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void Draw(Graphics& gfx) const noxnd;
	void Bind(Graphics& gfx) const noexcept;
private:
	struct DirectionLightCbuf
	{
		DirectX::XMFLOAT3 Dir;
		float Intensity;
		DirectX::XMFLOAT3 Color;
		float padding;
	};
private:
	DirectionLightCbuf cbData;
	mutable Bind::PixelConstantBuffer<DirectionLightCbuf> cbuf;

};