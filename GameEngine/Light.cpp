#include "Light.h"
#include "imgui/imgui.h"

Light::Light(Graphics& gfx, int numD , int numP ,int numS)
	:
	m_DirLightNum(numD),
	m_PointLightNum(numP),
	m_SpotLightNum(numS),
	cbuf(gfx)
{
	for (int i = 0; i < m_PointLightNum + m_SpotLightNum; i++)
	{
		mesh.push_back(std::make_shared<SolidSphere>(gfx,0.5f));
	}
	Reset();
}

void Light::SpawnControlWindow() noexcept
{
}

void Light::Reset() noexcept
{
	ResetDirectionLight();
	ResetPointLight();
	ResetSpotLight();
}



void Light::ResetDirectionLight() noexcept
{
	for (int i = 0; i < m_DirLightNum; i++)
	{
		lightData[i].direction = { 0.57735f, -0.57735f, 0.57735f };
		lightData[i].diffColor = { 0.6f, 0.6f, 0.6f };
		lightData[i].specColor = { 1.0f,1.0f,1.0f };
		lightData[i].ambient = { 0.05f,0.05f,0.05f };
	}
}

void Light::ResetPointLight() noexcept
{
	for (int i = m_DirLightNum; i < m_DirLightNum + m_PointLightNum; i++)
	{
		lightData[i].ambient = { 0.05f,0.05f,0.05f };
		lightData[i].position = { 0.0f,9.0f,0.0f };
		lightData[i].specColor = { 1.0f,1.0f,1.0f };
		lightData[i].diffColor = { 0.6f, 0.6f, 0.6f };
		lightData[i].attConst = 1.0f;
		lightData[i].attLin = 0.045f;
		lightData[i].attQuad = 0.0075f;
	}
}
void Light::ResetSpotLight() noexcept
{
	for (int i = m_DirLightNum + m_PointLightNum; i < m_DirLightNum + m_PointLightNum + m_SpotLightNum; i++)
	{
		lightData[i].ambient = { 0.05f,0.05f,0.05f };
		lightData[i].position = { 0.0f,9.0f,0.0f };
		lightData[i].direction = { 0.57735f, -0.57735f, 0.57735f };
		lightData[i].specColor = { 1.0f,1.0f,1.0f };
		lightData[i].diffColor = { 0.6f, 0.6f, 0.6f };
		lightData[i].attConst = 1.0f;
		lightData[i].attLin = 0.045f;
		lightData[i].attQuad = 0.0075f;
		lightData[i].spotPower = 0.045f;
		lightData[i].spotAngle = 0.0075f;
		lightData[i].diffuseIntensity = 1.0f;
	}

}

void Light::Bind(Graphics& gfx) const noexcept
{
	for (int i = 0; i < m_DirLightNum + m_PointLightNum + m_SpotLightNum; i++)
	{
		cbuf.Update(gfx, lightData[i]);
		cbuf.Bind(gfx);
	}
}

void Light::Draw(Graphics& gfx) const noxnd
{
	for (int i = 0; i < m_PointLightNum + m_SpotLightNum; i++)
	{
		mesh[i]->SetPos(lightData[m_DirLightNum + i].position);
		mesh[i]->DrawIndexed(gfx);
	}
}
