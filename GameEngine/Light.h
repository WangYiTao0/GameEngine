#pragma once
#include "LightCommon.h"
#include "Graphics.h"
#include "SolidSphere.h"
#include "ConstantBuffers.h"
#include "ConditionalNoexcept.h"
#include "Camera.h"
#include "ViewPoint.h"
#include <array>
#include <set>
#include <optional>
#include "DynamicConstant.h"
#include "ConstantBuffersEx.h"

class Light
{
private:
	struct LightCB
	{
		LightCommon L[MaxLights];
	};
	struct ShadowCB
	{
		DirectX::XMMATRIX ShadowViewMatrix;
		DirectX::XMMATRIX ShadowProjMatrix;
		DirectX::XMMATRIX ShadowOrthoMatrix;
	};

public:
	//DirectlightNum,PointLightNum,SpotLightNum
	Light(Graphics& gfx, int numD = 1, int numP = 1, int numS = 1);
	Light(const Light& rhs) = default;
	~Light() = default;
	void SpawnLightManagerWindow(Graphics& gfx)noexcept;
	
	//ëÆê´ Attributes
	template<typename T>
	void SetLightAttributes(int lightID,T& lightData)
	{
		lightData.L[lightID].position = lightData.position ? lightData.position : DirectX::XMFLOAT3(0.0f, 9.0f, 0.0f);
		lightData.L[lightID].direction = { 0.0f, 0.0f, 1.0f };
		lightData.L[lightID].ambient = { 0.0f,0.0f,0.0f };
		lightData.L[lightID].diffColor = { 1.0f, 1.0f, 1.0f };
		lightData.L[lightID].specular = { 1.0f,1.0f,1.0f };

		lightData.L[lightID].attConst = 1.0f;
		lightData.L[lightID].attLin = 0.045f;
		lightData.L[lightID].attQuad = 0.09f;
		lightData.L[lightID].spotPower = 0.032f;
		lightData.L[lightID].cutOff = std::cos(DirectX::XMConvertToRadians(12.5f));
		lightData.L[lightID].outerCutOff = std::cos(DirectX::XMConvertToRadians(15.f));
		lightData.L[lightID].diffuseIntensity = 1.0f;
	}

	void Reset() noexcept;
	void Update(Graphics& gfx);
	void Draw(Graphics& gfx) const noxnd;
	void Bind(Graphics& gfx) const noexcept;

	void GenerateShadowMatrix(Graphics& gfx, int lightID);
	ShadowCB GetShadowMatrix();
private:
	void ResetDirectionLight(int lightID) noexcept;
	void ResetPointLight(int lightID) noexcept;
	void ResetSpotLight(int lightID) noexcept;

	void TurnOffLight(int lightID) noexcept;
	void TurnOnLight(int lightID) noexcept;


	void SpawnLightControlWindow(int lightId)  noexcept;
	bool SpawnDirLightWindow( int lightId)noexcept;
	bool SpawnPointLightWindow( int lightId)noexcept;
	bool SpawnSpotLightWindow( int lightId)noexcept;

	void DrawLightRange(Graphics& gfx, int lightId)noexcept;
	void DrawDirLighRange(Graphics& gfx, int lightId)noexcept;
	void DrawPointLightRange(Graphics& gfx, int lightId)noexcept;
	void DrawSpotLightRange(Graphics& gfx, int lightId)noexcept;



private:
	int LightIndex = 0;
	int lightId = 0;

	std::set<int> lightControlIds;
	std::unordered_map<int, std::string> lightMap;
	LightCommon lastState[MaxLights];
	bool isTurnoff[MaxLights] = { false };

	int m_DirLightNum;
	int m_PointLightNum;
	int m_SpotLightNum;

	LightCB lightData;
	ShadowCB shadowMatrix;
	mutable Bind::VertexConstantBuffer<ShadowCB> shadowVSCB;
	mutable Bind::PixelConstantBuffer<LightCB> lightCB;
	mutable std::vector<std::shared_ptr<SolidSphere>> mesh;
	
	Dcb::RawLayout lightCBLayout;
	Dcb::RawLayout ShadowConst;
	//Dcb::Buffer buf;
	//	buf(Dcb::Buffer(layout));

	ViewPoint m_ViewPoint;
};
