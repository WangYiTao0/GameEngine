#include "DirectionLight.h"
#include "imgui/imgui.h"

DirectionLight::DirectionLight(Graphics& gfx)
	:
	cbuf(gfx,5u)
{
	Reset();
}

void DirectionLight::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("DirectionLight"))
	{
		ImGui::Text("Color");
		ImGui::ColorEdit3("Color", &cbData.Color.x);


		ImGui::Text("Direction");
		ImGui::SliderFloat3("direction", &cbData.Dir.x,-1.0f,1.0f,"%.3f",1.0f);

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
	cbData.Color = DirectX::XMFLOAT3(0.2f, 0.2f, 0.2f);
	cbData.Dir = DirectX::XMFLOAT3(-0.577f, -0.577f, 0.577f);
}

void DirectionLight::Bind(Graphics& gfx) const noexcept
{
	cbuf.Update(gfx, cbData);
	cbuf.Bind(gfx);
}

void DirectionLight::Draw(Graphics& gfx) const noxnd
{

}
