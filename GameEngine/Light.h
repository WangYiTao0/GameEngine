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
	void Draw(Graphics& gfx) const noxnd;
	void Bind(Graphics& gfx) const noexcept;

	DirectX::XMMATRIX GetLightViewMatrix(int lightID)
	{
		float fFarPlane = GCamera3D->GetFarZ();
		DirectX::XMFLOAT3 eyePos = lightData.L[lightID].position;
		DirectX::XMVECTOR eyePosVec = DirectX::XMVectorSet(eyePos.x, eyePos.y, eyePos.z, 1.0f);
		DirectX::XMVECTOR normalizeLightDir = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&lightData.L[lightID].direction));
		DirectX::XMVECTOR targetPosVec = DirectX::XMVectorSet(eyePos.x + DirectX::XMVectorGetX(normalizeLightDir),
			eyePos.y + DirectX::XMVectorGetY(normalizeLightDir), eyePos.z + DirectX::XMVectorGetZ(normalizeLightDir), 1.0f);
		DirectX::XMVECTOR upVec = DirectX::XMVectorSet(0.0, 1.0f, 0.0f, 0.0f);
		DirectX::XMMATRIX lightViewMatrix = DirectX::XMMatrixLookAtLH(eyePosVec, targetPosVec, upVec);

		return lightViewMatrix;
	}


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

private:
	struct LightCB
	{
		LightCommon L[MaxLights];
	};
	int LightIndex = 0;
	int lightId = 0;

	std::set<int> lightControlIds;

	LightCommon lastState[MaxLights];
	bool isTurnoff[MaxLights] = { false };

	DirectX::XMMATRIX LightViewMatrix;
	DirectX::XMMATRIX LightProjViewMatrix;
	DirectX::XMMATRIX LightOrthoMatrix;

	int m_DirLightNum;
	int m_PointLightNum;
	int m_SpotLightNum;

	LightCB lightData;
	//LightCB lightData[MaxLights];
	mutable Bind::PixelConstantBuffer<LightCB> cbuf;
	mutable std::vector<std::shared_ptr<SolidSphere>> mesh;
	//mutable SolidSphere *mesh[];

	std::unordered_map<int, std::string> lightMap;
};
