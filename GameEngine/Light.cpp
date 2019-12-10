#include "Light.h"
#include "imgui/imgui.h"
#include "Camera.h"

Light::Light(Graphics& gfx, int numD, int numP, int numS)
	:
	m_DirLightNum(numD),
	m_PointLightNum(numP),
	m_SpotLightNum(numS),
	cbuf(gfx, 1u)
{
	for (int i = 0; i < m_PointLightNum + m_SpotLightNum; i++)
	{
		mesh.push_back(std::make_shared<SolidSphere>(gfx, 0.5f));
	}

	LightIndex = m_DirLightNum + m_PointLightNum + m_SpotLightNum;

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

		}

	}
	ImGui::End();

	SpawnLightControlWindow();
}

void Light::SpawnLightControlWindow() noexcept
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

void Light::ResetDirectionLight(int lightID) noexcept
{
	lightData.L[lightID].direction = { -0.2f, -1.0f, 0.3f };
	lightData.L[lightID].ambient = { 0.05f,0.05f,0.05f };
	lightData.L[lightID].diffColor = { 0.4f, 0.4f, 0.4f };
	lightData.L[lightID].specular = { 0.5f,0.5f,0.5f };
}

void Light::ResetPointLight(int lightID) noexcept
{
	lightData.L[lightID].position = { 0.0f,9.0f,0.0f };
	lightData.L[lightID].ambient = { 0.05f,0.05f,0.05f };	
	lightData.L[lightID].diffColor = { 0.8f, 0.8f, 0.8f };
	lightData.L[lightID].specular = { 1.0f,1.0f,1.0f };

	lightData.L[lightID].attConst = 1.0f;
	lightData.L[lightID].attLin = 0.045f;
	lightData.L[lightID].attQuad = 0.0075f;


}
void Light::ResetSpotLight(int lightID) noexcept
{	
	lightData.L[lightID].position = { 0.0f,9.0f,0.0f };
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

}

bool Light::SpawnDirLightWindow(int lightId) noexcept
{
	bool dirty = false;
	bool open = true;

	if (ImGui::Begin(lightMap.find(lightId)->second.c_str(), &open))
	{
		ImGui::Text("Direction");
		ImGui::SliderFloat("Direction roll", &lightData.L[lightId].direction.x, DirectX::XMConvertToRadians(-180.0f), DirectX::XMConvertToRadians(180.0f));
		ImGui::SliderFloat("Direction pitch", &lightData.L[lightId].direction.y, DirectX::XMConvertToRadians(-180.0f), DirectX::XMConvertToRadians(180.0f));
		ImGui::SliderFloat("Direction yaw", &lightData.L[lightId].direction.z, DirectX::XMConvertToRadians(-180.0f), DirectX::XMConvertToRadians(180.0f));

		ImGui::Text("Light Color");
		ImGui::ColorEdit3("Diffuse Color", &lightData.L[lightId].diffColor.x);
		ImGui::ColorEdit3("Ambient", &lightData.L[lightId].ambient.x);
		ImGui::ColorEdit3("Specular", &lightData.L[lightId].specular.x);

		if (ImGui::Button("Reset"))
		{
			ResetDirectionLight(lightId);
		}
	}
	ImGui::End();

	return open;
}

bool Light::SpawnPointLightWindow(int lightId) noexcept
{
	bool dirty = false;
	bool open = true;
	if (ImGui::Begin(lightMap.find(lightId)->second.c_str(), &open))
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
	}
	ImGui::End();

	return open;
}

bool Light::SpawnSpotLightWindow(int lightId) noexcept
{
	bool dirty = false;
	bool open = true;
	if (ImGui::Begin(lightMap.find(lightId)->second.c_str(), &open))
	{
		ImGui::Text("Direction");
		ImGui::SliderFloat("Direction roll", &lightData.L[lightId].direction.x, DirectX::XMConvertToRadians(-180.0f), DirectX::XMConvertToRadians(180.0f));
		ImGui::SliderFloat("Direction pitch", &lightData.L[lightId].direction.y, DirectX::XMConvertToRadians(-180.0f), DirectX::XMConvertToRadians(180.0f));
		ImGui::SliderFloat("Direction yaw", &lightData.L[lightId].direction.z, DirectX::XMConvertToRadians(-180.0f), DirectX::XMConvertToRadians(180.0f));


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
	}
	ImGui::End();

	return open;
}

void Light::Bind(Graphics& gfx) const noexcept
{
	//for (int i = 0; i < m_DirLightNum + m_PointLightNum + m_SpotLightNum; i++)
	//{
	//
	//}	
	cbuf.Update(gfx, lightData);
	cbuf.Bind(gfx);

}

void Light::Draw(Graphics& gfx) const noxnd
{
	for (int i = 0; i < m_PointLightNum + m_SpotLightNum; i++)
	{
		mesh[i]->SetPos(lightData.L[m_DirLightNum + i].position);
		mesh[i]->DrawIndexed(gfx);
	}
}
