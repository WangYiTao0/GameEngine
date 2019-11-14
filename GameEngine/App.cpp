#include "App.h"
#include <memory>
#include <shellapi.h>
#include <algorithm>
#include <sstream>
#include "MathHelper.h"
#include "Surface.h"
#include "GDIPlusManager.h"
#include "imgui/imgui.h"
#include "TexturePreprocessor.h"
#include "ModelScene.h"
#include "GeometryScene.h"
#include "ShapesScene.h"
#include "PhysicScene.h"
#include "ECS.hpp"

namespace dx = DirectX;

const int width = 1600;
const int height= 900;
const float nearZ = 0.1f;
const float farZ = 4000.0f;
GDIPlusManager gdipm;

App::App(const std::string& commandLine)
	:
	commandLine(commandLine),
	wnd(width, height, "Game Engine"),
	pointLight(wnd.Gfx()),
	directionLight(wnd.Gfx()),
	cam(wnd.Gfx())
{
	// Create the cpu object.
	
	// Initialize the cpu object.
	m_Cpu.Initialize();
	// makeshift cli for doing some preprocessing bullshit (so many hacks here)
	if (this->commandLine != "")
	{
		int nArgs;
		const auto pLineW = GetCommandLineW();
		const auto pArgs = CommandLineToArgvW(pLineW, &nArgs);
		if (nArgs >= 3 && std::wstring(pArgs[1]) == L"--twerk-objnorm")
		{
			const std::wstring pathInWide = pArgs[2];
			TexturePreprocessor::FlipYAllNormalMapsInObj(
				std::string(pathInWide.begin(), pathInWide.end())
			);
			throw std::runtime_error("Normal maps all processed successfully. Just kidding about that whole runtime error thing.");
		}
		else if (nArgs >= 3 && std::wstring(pArgs[1]) == L"--twerk-flipy")
		{
			const std::wstring pathInWide = pArgs[2];
			const std::wstring pathOutWide = pArgs[3];
			TexturePreprocessor::FlipYNormalMap(
				std::string(pathInWide.begin(), pathInWide.end()),
				std::string(pathOutWide.begin(), pathOutWide.end())
			);
			throw std::runtime_error("Normal map processed successfully. Just kidding about that whole runtime error thing.");
		}
		else if (nArgs >= 4 && std::wstring(pArgs[1]) == L"--twerk-validate")
		{
			const std::wstring minWide = pArgs[2];
			const std::wstring maxWide = pArgs[3];
			const std::wstring pathWide = pArgs[4];
			TexturePreprocessor::ValidateNormalMap(
				std::string(pathWide.begin(), pathWide.end()), std::stof(minWide), std::stof(maxWide)
			);
			throw std::runtime_error("Normal map validated successfully. Just kidding about that whole runtime error thing.");
		}
	}

	lightType = LightType::PointLightType;
	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveFovLH(PI / 3.0f, 
		static_cast<float>(width) / static_cast<float>(height), 
		nearZ, farZ));

	
	//scenes.push_back(std::make_unique<ModelScene>(wnd.Gfx()));
	scenes.push_back(std::make_unique<GeometryScene>(wnd.Gfx()));
	scenes.push_back(std::make_unique<ShapesScene>(wnd.Gfx()));
	scenes.push_back(std::make_unique<PhysicScene>(wnd.Gfx()));
	//scenes.push_back(std::make_unique<PhysicScene>(wnd.Gfx()));
	curScene = scenes.begin();
	OutoutSceneName();
}

int App::Go()
{
	while (true)
	{
		// process all messages pending, but to not block for new messages
		if (const auto ecode = Window::ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		DoFrame();
	}
}

App::~App()
{
}

void App::HandleInput(float dt)
{
	while (const auto e = wnd.kbd.ReadKey())
	{
		if (!e->IsPress())
		{
			continue;
		}
		// cycle through scenes when tab is pressed
		//if (e->GetCode() == VK_TAB && e->IsPress())
		//{
		//	CycleScenes();
		//}
		switch (e->GetCode())
		{
		case VK_TAB:
			CycleScenes();
			break;

		case VK_ESCAPE:
			if (wnd.CursorEnabled())
			{
				wnd.DisableCursor();
				wnd.mouse.EnableRaw();
			}
			else
			{
				wnd.EnableCursor();
				wnd.mouse.DisableRaw();
			}
			break;
		case VK_F1:
			showDemoWindow = true;
			break;
		case VK_NUMPAD0:
			lightType = LightType::DirectionLightType;
			break;
		case VK_NUMPAD1:
			lightType = LightType::PointLightType;
			break;
		}
	}

	if (!wnd.CursorEnabled())
	{
		if (wnd.kbd.KeyIsPressed('W'))
		{
			cam.Translate({ 0.0f,0.0f,dt });
		}
		if (wnd.kbd.KeyIsPressed('A'))
		{
			cam.Translate({ -dt,0.0f,0.0f });
		}
		if (wnd.kbd.KeyIsPressed('S'))
		{
			cam.Translate({ 0.0f,0.0f,-dt });
		}
		if (wnd.kbd.KeyIsPressed('D'))
		{
			cam.Translate({ dt,0.0f,0.0f });
		}
		if (wnd.kbd.KeyIsPressed('R'))
		{
			cam.Translate({ 0.0f,dt,0.0f });
		}
		if (wnd.kbd.KeyIsPressed('F'))
		{
			cam.Translate({ 0.0f,-dt,0.0f });
		}
	}

	while (const auto delta = wnd.mouse.ReadRawDelta())
	{
		if (!wnd.CursorEnabled())
		{
			cam.Rotate((float)delta->x, (float)delta->y);
		}
	}
}

void App::update(float dt)
{
	// update scene
	(*curScene)->Update(dt);
	m_Cpu.Frame();
}

void App::Draw()
{	
	// draw scene
	(*curScene)->Draw();	
	
	// imgui windows
	cam.SpawnControlWindow();
	SpawnEngineStateWindow();
}

void App::DoFrame()
{
	const auto dt = timer.Mark()* speed_factor;
	
	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);

	wnd.Gfx().SetCameraViewMatirx(cam.GetViewMatrix());

	switch (lightType)
	{
	case App::LightType::DirectionLightType:
		{
			directionLight.Bind(wnd.Gfx());
			directionLight.Draw(wnd.Gfx());
			directionLight.SpawnControlWindow();
			break;
		}
	case App::LightType::PointLightType:
		{
			pointLight.Bind(wnd.Gfx(), cam.GetViewMatrix());
			pointLight.Draw(wnd.Gfx());
			pointLight.SpawnControlWindow();
			break;
		}
	case App::LightType::SpotLightType:
		break;
	case App::LightType::MaxType:
		break;
	default:
		break;
	}

	HandleInput(dt);
	update(wnd.kbd.KeyIsPressed(VK_SPACE) ? 0.0f : dt);
	Draw();

	// present
	wnd.Gfx().EndFrame();
}

void App::SpawnEngineStateWindow()
{
	if (showDemoWindow)
	{
		ImGui::ShowDemoWindow(&showDemoWindow);
	}
	std::string cpuPrecentage = std::to_string(m_Cpu.GetCpuPercentage()) + "%";
	// imgui window to control simulation speed
	if (ImGui::Begin("Game State"))
	{
		ImGui::Text("CPU: %s %",cpuPrecentage.c_str());
		ImGui::SliderFloat("Speed Factor", &speed_factor, 0.0f, 6.0f, "%.4f", 3.2f);
		// 1000/fps  = ms render one frame time
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Status: %s", wnd.kbd.KeyIsPressed(VK_SPACE) ? "PAUSED" : "RUNNING (hold spacebar to pause)");
	}
	ImGui::End();

	ImGui::BeginMainMenuBar();
	ImGui::Button("File");
	ImGui::EndMainMenuBar();

}

void App::OutoutSceneName() const
{
	std::stringstream ss;
	const std::string SceneName((*curScene)->GetName().size() + 4, '*');

	ss << SceneName << std::endl
		<< "* " << (*curScene)->GetName() << " *" << std::endl
		<< SceneName << std::endl;
	OutputDebugStringA(ss.str().c_str());
}

void App::CycleScenes()
{
	if (++curScene == scenes.end())
	{
		curScene = scenes.begin();
	}
	OutoutSceneName();
}







