#include "Light.h"
#include "imgui/imgui.h"
#include "Camera.h"
#include "MathHelper.h"

Light::Light(Graphics& gfx, int numD, int numP, int numS)
	:
	m_DirLightNum(numD),
	m_PointLightNum(numP),
	m_SpotLightNum(numS),
	lightCB(gfx, 1u),
	shadowCB(gfx,1u)//VB
{	
	LightIndex = m_DirLightNum + m_PointLightNum + m_SpotLightNum;
	for (int i = 0; i < LightIndex; i++)
	{
		mesh.push_back(std::make_shared<SolidSphere>(gfx, 0.5f));
	}


	for (lightId = 0; lightId < LightIndex; lightId++)
	{
		if (lightId < m_DirLightNum)
		{
			lightMap.insert({ lightId, "DirectionLight" + std::to_string(lightId + 1) });
		}
		if (lightId >= m_DirLightNum && lightId < m_DirLightNum + m_PointLightNum)
		{
			lightMap.insert({ lightId, "PointLight" + std::to_string(lightId - m_DirLightNum + 1) });
		}
		if (lightId >= m_DirLightNum + m_PointLightNum && lightId < m_DirLightNum + m_PointLightNum + m_SpotLightNum)
		{
			lightMap.insert({ lightId, "SpotLight" + std::to_string(lightId - m_DirLightNum - m_PointLightNum + 1) });
		}
	}
	Reset();
}

void Light::SpawnLightManagerWindow(Graphics& gfx) noexcept
{
	if (ImGui::Begin("Light"))
	{
		using namespace std::string_literals;
		auto preview = lightMap.find(lightId) != lightMap.end() ?
			lightMap.find(lightId)->second.c_str() : "Choose Light..."s;
		// LightIndex ? std::to_string(LightIndex) : "Choose LightId..."s;
		if (ImGui::BeginCombo("Light Number", preview.c_str()))
		{
			for (int i = 0; i < LightIndex; i++)
			{
				const bool selected = lightId == i;
				if (ImGui::Selectable(lightMap.find(i)->second.c_str(), selected))
				{
					lightId = i;
				}
				if (selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		if (ImGui::Button("Spawn Control Window") && lightId)
		{
			lightControlIds.insert(lightId);
		}
	}
	ImGui::End();

	SpawnLightControlWindow(lightId);
	
}
void Light::SpawnLightControlWindow(int lightId) noexcept
{

	if (lightId < m_DirLightNum)
	{
		SpawnDirLightWindow(lightId);
	}
	if (lightId >= m_DirLightNum && lightId < m_DirLightNum + m_PointLightNum)
	{
		SpawnPointLightWindow(lightId);
	}
	if (lightId >= m_DirLightNum + m_PointLightNum && lightId < m_DirLightNum + m_PointLightNum + m_SpotLightNum)
	{
		SpawnSpotLightWindow(lightId);
	}
}
void Light::Reset() noexcept
{
	for (int i = 0; i < m_DirLightNum; i++)
	{
		ResetDirectionLight(i);

	}
	for (int i = m_DirLightNum; i < m_DirLightNum + m_PointLightNum; i++)
	{
		ResetPointLight(i);
	}
	for (int i = m_DirLightNum + m_PointLightNum; i < m_DirLightNum + m_PointLightNum + m_SpotLightNum; i++)
	{
		ResetSpotLight(i);
	}
}
void Light::Update(Graphics& gfx)
{
	GenerateShadowMatrix(gfx, 0);
	shadowMatrix = GetShadowMatrix();
}
void Light::ResetDirectionLight(int lightID) noexcept
{
	//for shadow Calculate 
	lightData.L[lightID].position = { 0.0f,9.0f,0.0f };

	lightData.L[lightID].direction = { -0.2f, -1.0f, 0.3f };
	lightData.L[lightID].ambient = { 0.05f,0.05f,0.05f };
	lightData.L[lightID].diffColor = { 0.4f, 0.4f, 0.4f };
	lightData.L[lightID].specular = { 0.5f,0.5f,0.5f };
	isTurnoff[lightId] = false;
}
void Light::ResetPointLight(int lightID) noexcept
{
	//for shadow Calculate 
	lightData.L[lightID].direction = { 0.0f, DirectX::XMConvertToRadians(-90.0f), 0.0f };

	lightData.L[lightID].position = { 0.0f,9.0f,0.0f };
	lightData.L[lightID].ambient = { 0.05f,0.05f,0.05f };	
	lightData.L[lightID].diffColor = { 0.8f, 0.8f, 0.8f };
	lightData.L[lightID].specular = { 1.0f,1.0f,1.0f };

	lightData.L[lightID].attConst = 1.0f;
	lightData.L[lightID].attLin = 0.045f;
	lightData.L[lightID].attQuad = 0.0075f;
	isTurnoff[lightId] = false;
}
void Light::ResetSpotLight(int lightID) noexcept
{	
	lightData.L[lightID].position = { 0.0f,9.0f,0.0f };
	lightData.L[lightID].direction = { 0.0f, DirectX::XMConvertToRadians(-90.0f), 0.0f };
	lightData.L[lightID].ambient = { 0.0f,0.0f,0.0f };
	lightData.L[lightID].diffColor = { 1.0f, 1.0f, 1.0f };
	lightData.L[lightID].specular = { 1.0f,1.0f,1.0f };

	lightData.L[lightID].attConst = 1.0f;
	lightData.L[lightID].attLin = 0.045f;
	lightData.L[lightID].attQuad = 0.09f;
	lightData.L[lightID].spotPower = 0.032f;
	lightData.L[lightID].cutOff = std::cos(DirectX::XMConvertToRadians(12.5f));
	lightData.L[lightID].outerCutOff = std::cos(DirectX::XMConvertToRadians(15.f));
	isTurnoff[lightId] = false;
}

void Light::TurnOffLight(int lightID) noexcept
{
	if (!isTurnoff[lightId])
	{
		lastState[lightID] = lightData.L[lightID];
		lightData.L[lightID].ambient = { 0.0f,0.0f,0.0f };
		lightData.L[lightID].diffColor = { 0.f, 0.f, 0.f };
		lightData.L[lightID].specular = { 0.f,0.f,0.f };
		isTurnoff[lightId] = true;
	}
	


	
}

void Light::TurnOnLight(int lightID) noexcept
{
	if (isTurnoff[lightId])
	{
		lightData.L[lightID] = lastState[lightID];
		isTurnoff[lightId] = false;
	}
}

bool Light::SpawnDirLightWindow(int lightId) noexcept
{
	bool dirty = false;
	bool open = true;
	if (open)
	{
		if (!ImGui::Begin(lightMap.find(lightId)->second.c_str(), &open))
		{
			ImGui::End();
		}
		else 
		{
			ImGui::Text("Direction");
			ImGui::SliderFloat("Direction roll", &lightData.L[lightId].direction.x, -180*MH::oneRad, 180*MH::oneRad);
			ImGui::SliderFloat("Direction pitch", &lightData.L[lightId].direction.y, -180*MH::oneRad, 180*MH::oneRad);
			ImGui::SliderFloat("Direction yaw", &lightData.L[lightId].direction.z, -180*MH::oneRad, 180*MH::oneRad);

			ImGui::Text("Light Color");
			ImGui::ColorEdit3("Diffuse Color", &lightData.L[lightId].diffColor.x);
			ImGui::ColorEdit3("Ambient", &lightData.L[lightId].ambient.x);
			ImGui::ColorEdit3("Specular", &lightData.L[lightId].specular.x);

			if (ImGui::Button("Reset"))
			{
				ResetDirectionLight(lightId);
			}
			if (ImGui::Button("Turn Off"))
			{
				TurnOffLight(lightId);
			}
			if (ImGui::Button("Turn On"))
			{
				TurnOnLight(lightId);
			}
			ImGui::End();
		}
	
	}
	return open;
}

bool Light::SpawnPointLightWindow(int lightId) noexcept
{
	bool dirty = false;
	bool open = true;
	if (!ImGui::Begin(lightMap.find(lightId)->second.c_str(), &open))
	{
		ImGui::End();
	}
	else
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &lightData.L[lightId].position.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &lightData.L[lightId].position.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &lightData.L[lightId].position.z, -60.0f, 60.0f, "%.1f");

		ImGui::Text("Light Color");
		ImGui::ColorEdit3("Diffuse Color", &lightData.L[lightId].diffColor.x);
		ImGui::ColorEdit3("Ambient", &lightData.L[lightId].ambient.x);
		ImGui::ColorEdit3("Specular", &lightData.L[lightId].specular.x);

		ImGui::Text("Falloff");
		ImGui::SliderFloat("Constant", &lightData.L[lightId].attConst, 0.05f, 10.0f, "%.2f", 4);
		ImGui::SliderFloat("Linear", &lightData.L[lightId].attLin, 0.0001f, 4.0f, "%.4f", 8);
		ImGui::SliderFloat("Quadratic", &lightData.L[lightId].attQuad, 0.0000001f, 10.0f, "%.7f", 10);

		if (ImGui::Button("Reset"))
		{
			ResetPointLight(lightId);
		}
		if (ImGui::Button("Turn Off"))
		{
			TurnOffLight(lightId);
		}
		if (ImGui::Button("Turn On"))
		{
			TurnOnLight(lightId);
		}
	}
	ImGui::End();
	return open;
}

bool Light::SpawnSpotLightWindow(int lightId) noexcept
{
	bool dirty = false;
	bool open = true;
	if (!ImGui::Begin(lightMap.find(lightId)->second.c_str(), &open))
	{
		ImGui::End();
	}
	else
	{
		ImGui::Text("Direction");
		ImGui::SliderFloat("Dir Pitch", &lightData.L[lightId].direction.x, -180*MH::oneRad, 180*MH::oneRad);
		ImGui::SliderFloat("Dir Yaw", &lightData.L[lightId].direction.y, -180*MH::oneRad, 180*MH::oneRad);
		ImGui::SliderFloat("Dir Roll", &lightData.L[lightId].direction.z, -180*MH::oneRad, 180*MH::oneRad);

		ImGui::Text("Position");
		ImGui::SliderFloat("X", &lightData.L[lightId].position.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &lightData.L[lightId].position.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &lightData.L[lightId].position.z, -60.0f, 60.0f, "%.1f");

		ImGui::Text("Light Color");
		ImGui::ColorEdit3("Diffuse Color", &lightData.L[lightId].diffColor.x);
		ImGui::ColorEdit3("Ambient", &lightData.L[lightId].ambient.x);
		ImGui::ColorEdit3("Specular", &lightData.L[lightId].specular.x);

		ImGui::Text("Falloff");
		ImGui::SliderFloat("Constant", &lightData.L[lightId].attConst, 0.05f, 10.0f, "%.2f", 4);
		ImGui::SliderFloat("Linear", &lightData.L[lightId].attLin, 0.0001f, 4.0f, "%.4f", 8);
		ImGui::SliderFloat("Quadratic", &lightData.L[lightId].attQuad, 0.0000001f, 10.0f, "%.7f", 10);
		ImGui::SliderFloat("CutOff", &lightData.L[lightId].cutOff, DirectX::XMConvertToRadians(0.01f), DirectX::XMConvertToRadians(80.0f), "%.4f");
		ImGui::SliderFloat("OuterCutOff", &lightData.L[lightId].outerCutOff, DirectX::XMConvertToRadians(0.02f), DirectX::XMConvertToRadians(90.0f), "%.4f");

		if (ImGui::Button("Reset"))
		{
			ResetSpotLight(lightId);
		}
		if (ImGui::Button("Turn Off"))
		{
			TurnOffLight(lightId);
		}
		if (ImGui::Button("Turn On"))
		{
			TurnOnLight(lightId);
		}
	}
	ImGui::End();

	return open;
}

void Light::Bind(Graphics& gfx) const noexcept
{
	lightCB.Update(gfx, lightData);
	lightCB.Bind(gfx);

	shadowCB.Update(gfx, shadowMatrix);
	shadowCB.Bind(gfx);
}

void Light::GenerateShadowMatrix(Graphics& gfx, int lightID)
{	
	m_ViewPoint.SetPostion(lightData.L[lightID].position);
	m_ViewPoint.SetLookAt(lightData.L[lightID].direction);
	m_ViewPoint.SetScreen(gfx.GetScreenWidth(), gfx.GetScreenHeight());
	m_ViewPoint.SetProjectionParameters(GCamera3D->GetFov(), gfx.GetAspect(), GCamera3D->GetNearZ(), GCamera3D->GetFarZ());
	m_ViewPoint.GenerateViewMatrix();
	m_ViewPoint.GenerateProjMatrix();
	m_ViewPoint.GenerateOrthoMatrix();
}

Light::ShadowCB Light::GetShadowMatrix()
{
	using namespace DirectX;
	const auto s_view = m_ViewPoint.GetViewMatrix();
	const auto s_proj = m_ViewPoint.GetProjMatrix();
	const auto s_ortho = m_ViewPoint.GetOrthoMatrix();
	
	return {
		XMMatrixTranspose(s_view),
		XMMatrixTranspose(s_proj),
		XMMatrixTranspose(s_ortho)	};
}

//ShadowCB Light::GetShadowMatrix(int lightID)
//{
//	float fFarPlane = GCamera3D->GetFarZ();
//	DirectX::XMFLOAT3 eyePos = lightData.L[lightID].position;
//	DirectX::XMVECTOR eyePosVec = DirectX::XMVectorSet(eyePos.x, eyePos.y, eyePos.z, 1.0f);
//	DirectX::XMVECTOR normalizeLightDir = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&lightData.L[lightID].direction));
//	DirectX::XMVECTOR targetPosVec = DirectX::XMVectorSet(eyePos.x + DirectX::XMVectorGetX(normalizeLightDir),
//		eyePos.y + DirectX::XMVectorGetY(normalizeLightDir), eyePos.z + DirectX::XMVectorGetZ(normalizeLightDir), 1.0f);
//	DirectX::XMVECTOR upVec = DirectX::XMVectorSet(0.0, 1.0f, 0.0f, 0.0f);
//	DirectX::XMMATRIX lightViewMatrix = DirectX::XMMatrixLookAtLH(eyePosVec, targetPosVec, upVec);
//
//	return lightViewMatrix;
//}

void Light::Draw(Graphics& gfx) const noxnd
{

	for (int i = 0; i < LightIndex; i++)
	{
		if (!isTurnoff[i])
		{
			mesh[i]->SetPos(lightData.L[i].position);
			mesh[i]->DrawIndexed(gfx);
		}


	}
}
