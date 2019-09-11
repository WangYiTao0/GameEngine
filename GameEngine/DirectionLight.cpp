#include "DirectionLight.h"
#include "imgui/imgui.h"

DirectionLight::DirectionLight(Graphics& gfx)
	:
	cbuf(gfx)
{
	Reset();
}

void DirectionLight::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("DirectionLight"))
	{
		ImGui::Text("Color");
		ImGui::ColorEdit4("ambient", &cbData.ambient.x);
		ImGui::ColorEdit4("diffuse", &cbData.diffuse.x);
		ImGui::ColorEdit4("specular", &cbData.specular.x);

		ImGui::Text("Direction");
		ImGui::SliderFloat3("direction", &cbData.direction.x,-1.0f,1.0f,"%.3f",1.0f);

		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}

	ImGui::End();
}

void DirectionLight::Reset() noexcept
{
	cbData = {};
	cbData.ambient = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	cbData.diffuse = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	cbData.specular = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	cbData.direction = DirectX::XMFLOAT3(-0.577f, -0.577f, 0.577f);
}

void DirectionLight::Bind(Graphics& gfx) const noexcept
{
	cbuf.Update(gfx, cbData);
	cbuf.Bind(gfx);
}

void DirectionLight::Draw(Graphics& gfx) const noxnd
{

}
