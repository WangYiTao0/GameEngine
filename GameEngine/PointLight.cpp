#include "PointLight.h"
#include "imgui/imgui.h"

PointLight::PointLight(Graphics& gfx, float radius)
	:
	mesh(gfx, radius),
	cbuf(gfx,1u)
{
	Reset();
}

void PointLight::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("PointLight"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &cbData.position.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &cbData.position.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &cbData.position.z, -60.0f, 60.0f, "%.1f");

		ImGui::Text("Intensity/Color");
		ImGui::SliderFloat("Intensity", &cbData.Intensity, 0.01f, 2.0f, "%.2f", 2);
		ImGui::ColorEdit3("Ambient", &cbData.ambient.x);
		ImGui::ColorEdit3("Diffuse", &cbData.diffuse.x);
		ImGui::ColorEdit3("Specular", &cbData.specular.x);

		ImGui::Text("Falloff");
		ImGui::SliderFloat("Constant", &cbData.attConst, 0.05f, 10.0f, "%.2f", 4);
		ImGui::SliderFloat("Linear", &cbData.attLin, 0.0001f, 4.0f, "%.4f", 8);
		ImGui::SliderFloat("Quadratic", &cbData.attQuad, 0.0000001f, 10.0f, "%.7f", 10);


		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Reset() noexcept
{
	cbData = {};
	cbData.position = { 0.0f,9.0f,0.0f };
	cbData.ambient = { 0.05f,0.05f,0.05f };
	cbData.diffuse = { 1.0f,1.0f,1.0f };
	cbData.specular = { 1.0f,1.0f,1.0f };
	cbData.Intensity = 1.0f;
	cbData.attConst = 1.0f;
	cbData.attLin = 0.045f;
	cbData.attQuad = 0.0075f;

}

void PointLight::Draw(Graphics& gfx) const noxnd
{
	mesh.SetPos(cbData.position);
	mesh.Draw(gfx);
}

void PointLight::Bind(Graphics& gfx) const noexcept
{
	cbuf.Update(gfx, cbData);
	cbuf.Bind(gfx);
}
