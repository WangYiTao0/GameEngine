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
		DirectX::XMFLOAT4 ambient;
		DirectX::XMFLOAT4 diffuse;
		DirectX::XMFLOAT4 specular;
		DirectX::XMFLOAT3 direction;
		float pad;
	};
private:
	DirectionLightCbuf cbData;
	mutable Bind::PixelConstantBuffer<DirectionLightCbuf> cbuf;

};