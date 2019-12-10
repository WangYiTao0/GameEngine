#pragma once
#include "LightCommon.h"
#include "Graphics.h"
#include "SolidSphere.h"
#include "ConstantBuffers.h"
#include "ConditionalNoexcept.h"
#include "Camera.h"
#include <array>
#include <set>
#include <optional>

class Light
{
public:
	//DirectlightNum,PointLightNum,SpotLightNum
	Light(Graphics& gfx, int numD = 1, int numP = 1, int numS = 1);
	Light(const Light& rhs) = default;
	~Light() = default;
	void SpawnLightManagerWindow(Graphics& gfx)noexcept;
	
	void Reset() noexcept;
	void Draw(Graphics& gfx) const noxnd;
	void Bind(Graphics& gfx) const noexcept;

private:
	void ResetDirectionLight(int lightID) noexcept;
	void ResetPointLight(int lightID) noexcept;
	void ResetSpotLight(int lightID) noexcept;

	void SpawnLightControlWindow()  noexcept;
	bool SpawnDirLightWindow( int lightId)noexcept;
	bool SpawnPointLightWindow( int lightId)noexcept;
	bool SpawnSpotLightWindow( int lightId)noexcept;

private:
	 int m_DirLightNum;
	 int m_PointLightNum;
	 int m_SpotLightNum;
	

	LightCB lightData;
	//LightCB lightData[MaxLights];
	mutable Bind::PixelConstantBuffer<LightCB> cbuf;
	mutable std::vector<std::shared_ptr<SolidSphere>> mesh;
	//mutable SolidSphere *mesh[];
	int LightIndex = 0;
	int lightId = 0;
	std::unordered_map<int, std::string> lightMap;
};
