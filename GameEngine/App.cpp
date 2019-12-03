#include "App.h"
#include <memory>
#include <shellapi.h>
#include <algorithm>
#include <sstream>
#include "MathHelper.h"
#include "imgui/imgui.h"
#include "ModelScene.h"
#include "GeometryScene.h"
#include "ShapesScene.h"
#include "PhysicScene.h"

namespace dx = DirectX;




App::App()
	:
	wnd(screenWidth, screenHeight, "Game Engine"),
	pointLight(wnd.Gfx()),
	cam(wnd.Gfx()),
	rtt(wnd.Gfx(), screenWidth, screenHeight)

{
	tex2.SetPos({ 0.0f,screenHeight*3.0f / 4.0f,0.0f });
	// Create the cpu object.
	m_Cpu.Initialize();
	// makeshift cli for doing some preprocessing bullshit (so many hacks here)

	//2D Camera
	cam2D.SetOrtho(static_cast<float>(screenWidth), 
		static_cast<float>(screenHeight),
		0.0f, 1.0f);
	wnd.Gfx().SetOrtho(cam2D.GetOrthoMatrix());
	//3D Camera
	cam.Set3DProj(MathHelper::PI / 4.0f,
		static_cast<float>(screenWidth) / static_cast<float>(screenHeight),
		nearZ, farZ);
	wnd.Gfx().SetProjection(cam.GetProj());

	//scenes.push_back(std::make_unique<ModelScene>(wnd.Gfx()));
	scenes.push_back(std::make_unique<GeometryScene>(wnd.Gfx()));
	scenes.push_back(std::make_unique<ShapesScene>(wnd.Gfx()));
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
	//update Matrix
	wnd.Gfx().SetCamera2DWorldMatirx(cam2D.GetWorldMatrix());
	wnd.Gfx().SetCameraViewMatirx(cam.GetViewMatrix());


	//update point light
	pointLight.Bind(wnd.Gfx(), cam.GetViewMatrix());
	//update Gpu frame
	m_Cpu.Frame();
	// update scene
	(*curScene)->Update(dt);
	
}

void App::Draw()
{	
	RenderScene();

	// imgui windows
	cam.SpawnControlWindow();
	SpawnEngineStateWindow();
	pointLight.SpawnControlWindow();

	tex2.Draw(wnd.Gfx());
}

void App::DoFrame()
{
	const auto dt = timer.Mark()* speed_factor;

	//render scene to texture
	RenderToTexture();

	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);

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

void App::RenderToTexture()
{
	rtt.Bind(wnd.Gfx());

	RenderScene();

	wnd.Gfx().SetBackBufferRenderTarget();
}

void App::RenderScene()
{
	pointLight.Draw(wnd.Gfx());
	// draw scene
	(*curScene)->Draw();
}







