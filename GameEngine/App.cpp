#include "App.h"
#include <memory>
#include <algorithm>
#include "ChiliMath.h"
#include "Surface.h"
#include "GDIPlusManager.h"
#include "imgui/imgui.h"
#include "ModelScene.h"
#include "GeometryScene.h"
#include <sstream>

namespace dx = DirectX;

const int width = 1600;
const int  height= 900;
GDIPlusManager gdipm;

App::App()
	:
	wnd(width, height, "Game Engine"),
	light(wnd.Gfx()),

	bs1(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 10),
	bs2(DirectX::XMFLOAT3(21.0f, 0.0f, 0.0f), 10)
{

	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));

	scenes.push_back(std::make_unique<ModelScene>(wnd.Gfx()));
	scenes.push_back(std::make_unique<GeometryScene>(wnd.Gfx()));
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
	// update scene
	(*curScene)->Update(wnd.kbd, wnd.mouse, dt);
}

void App::Draw()
{	
	
	light.Draw(wnd.Gfx());
	// draw scene


	(*curScene)->Draw(wnd.Gfx());	
	

	// imgui windows
	cam.SpawnControlWindow();
	light.SpawnControlWindow();
	ShowImguiDemoWindow();


}

void App::DoFrame()
{
	const auto dt = timer.Mark()* speed_factor;
	
	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);

	wnd.Gfx().SetCamera(cam.GetMatrix());
	light.Bind(wnd.Gfx(),cam.GetMatrix());

	HandleInput(dt);
	update(dt);
	Draw();

	// present
	wnd.Gfx().EndFrame();
}

void App::ShowImguiDemoWindow()
{
	if (showDemoWindow)
	{
		ImGui::ShowDemoWindow(&showDemoWindow);
	}
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







