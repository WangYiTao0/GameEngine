#pragma once
#include "LightCommon.h"
#include "Graphics.h"
#include "SolidSphere.h"
#include "ConstantBuffers.h"
#include "ConditionalNoexcept.h"
#include "Camera.h"
#include <array>

class Light
{
public:
	//DirectlightNum,PointLightNum,SpotLightNum
	Light(Graphics& gfx, int numD = 1, int numP = 1, int numS = 1);
	Light(const Light& rhs) = default;
	~Light() = default;
	void SpawnControlWindow()  noexcept;
	void Reset() noexcept;
	void Draw(Graphics& gfx) const noxnd;
	void Bind(Graphics& gfx) const noexcept;

private:
	void ResetDirectionLight() noexcept;
	void ResetPointLight() noexcept;
	void ResetSpotLight() noexcept;
private:
	 int m_DirLightNum;
	 int m_PointLightNum;
	 int m_SpotLightNum;
	

	std::array<LightCB, MaxLights> lightData;
	//LightCB lightData[MaxLights];
	mutable Bind::PixelConstantBuffer<LightCB> cbuf;
	mutable std::vector<std::shared_ptr<SolidSphere>> mesh;
	//mutable SolidSphere *mesh[];
};
