#include "App.h"
#include <memory>
#include <shellapi.h>
#include <algorithm>
#include <sstream>
#include "MathHelper.h"
#include "wytEnum.h"
#include "imgui/imgui.h"
#include "SponzaScene.h"
#include "GeometryScene.h"
#include "ShapesScene.h"
#include "PhysicScene.h"
#include "PBRScene.h"
#include "TestDCB.h"
#include "DepthStencil.h"


namespace dx = DirectX;

std::shared_ptr<Bind::DepthBufferRT> App:: m_DepthRT = nullptr;

App::App()
	:
	wnd(screenWidth, screenHeight, "Game Engine")
{
	//TestDynamicConstant();

	// Create the cpu object.
	m_Cpu.Initialize();

	//2D Camera
	m_Camera2D.SetOrtho(static_cast<float>(screenWidth), 
		static_cast<float>(screenHeight),
		0.0f, 1.0f);
	wnd.Gfx().SetOrtho(m_Camera2D.GetOrthoMatrix());
	//3D Camera
	GCamera3D->Set3DProj(MH::PI / 4.0f,
		static_cast<float>(screenWidth) / static_cast<float>(screenHeight),
		nearZ, farZ);
	wnd.Gfx().SetProjection(GCamera3D->GetProj());

	InitDebugWindow();

	//m_Scenes.push_back(std::make_unique<SponzaScene>(wnd.Gfx()));
	//m_Scenes.push_back(std::make_unique<GeometryScene>(wnd.Gfx()));
	m_Scenes.push_back(std::make_unique<PBRScene>(wnd.Gfx()));
	//m_Scenes.push_back(std::make_unique<ShapesScene>(wnd.Gfx()));
	//m_Scenes.push_back(std::make_unique<PhysicScene>(wnd.Gfx()));

	m_CurScene = m_Scenes.begin();
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

void App::DoFrame()
{
	const auto dt = timer.Mark() * speed_factor;

	//if (enableRenderTarget)
	//{
		//render scene to texture
		RenderToTexture();
	//}
	wnd.Gfx().SetProjection(GCamera3D->GetProj());
	wnd.Gfx().SetCameraViewMatirx(GCamera3D->GetViewMatrix());

	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);

	HandleInput(dt);
	update(wnd.kbd.KeyIsPressed(VK_SPACE) ? 0.0f : dt);
	Draw();

	// present
	wnd.Gfx().EndFrame();
}

void App::HandleInput(float dt)
{
	while (const auto e = wnd.kbd.ReadKey())
	{
		if (!e->IsPress())
		{
			continue;
		}
		// cycle through m_Scenes when tab is pressed
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
		//case VK_F1:
		//	showDemoWindow = true;
		//	break;
		case VK_F1:
			enableRenderTarget = !enableRenderTarget;
			break;

		}
	}

	if (!wnd.CursorEnabled())
	{
		if (wnd.kbd.KeyIsPressed('W'))
		{
			GCamera3D->Translate({ 0.0f,0.0f,dt });
		}
		if (wnd.kbd.KeyIsPressed('A'))
		{
			GCamera3D->Translate({ -dt,0.0f,0.0f });
		}
		if (wnd.kbd.KeyIsPressed('S'))
		{
			GCamera3D->Translate({ 0.0f,0.0f,-dt });
		}
		if (wnd.kbd.KeyIsPressed('D'))
		{
			GCamera3D->Translate({ dt,0.0f,0.0f });
		}
		if (wnd.kbd.KeyIsPressed('R'))
		{
			GCamera3D->Translate({ 0.0f,dt,0.0f });
		}
		if (wnd.kbd.KeyIsPressed('F'))
		{
			GCamera3D->Translate({ 0.0f,-dt,0.0f });
		}
	}

	while (const auto delta = wnd.mouse.ReadRawDelta())
	{
		if (!wnd.CursorEnabled())
		{
			GCamera3D->Rotate((float)delta->x, (float)delta->y);
		}
	}
}

void App::update(float dt)
{	
	//update Gpu frame
	m_Cpu.Frame();
	//update View Matrix
	wnd.Gfx().SetCamera2DWorldMatirx(m_Camera2D.GetWorldMatrix());
	wnd.Gfx().SetCameraViewMatirx(GCamera3D->GetViewMatrix());


	
	// update scene
	(*m_CurScene)->Update(dt);
	
}

void App::Draw()
{
	RenderScene();

	// imgui windows
	GCamera3D->SpawnControlWindow();
	SpawnEngineStateWindow();


	if (enableRenderTarget)
	{
		//m_SmallScene->DrawIndexed(wnd.Gfx());
		m_ProjDepthMap->DrawIndexed(wnd.Gfx());
	}
}

void App::SpawnEngineStateWindow()
{
	//if (showDemoWindow)
	//{
	//	ImGui::ShowDemoWindow(&showDemoWindow);
	//}
	std::string cpuPrecentage = std::to_string(m_Cpu.GetCpuPercentage()) + "%";
	// imgui window to control simulation speed
	if (ImGui::Begin("Game State"))
	{
		ImGui::Text("CPU: %s %",cpuPrecentage.c_str());
		ImGui::SliderFloat("Speed Factor", &speed_factor, 0.0f, 6.0f, "%.4f", 3.2f);
		// 1000/fps  = ms render one frame time
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Status: %s", wnd.kbd.KeyIsPressed(VK_SPACE) ? "PAUSED" : "RUNNING (hold spacebar to pause)");
		ImGui::Text("F1 RenderTagert: %s", enableRenderTarget ? "On" : "OFF ");
		ImGui::Text("F2 SSAO	    : %s", enableRenderTarget ? "On" : "OFF ");
		ImGui::Text("F3 Bloom       : %s", enableRenderTarget ? "On" : "OFF ");
		//ImGui::Text("F4 RenderTagert: %s", wnd.kbd.KeyIsPressed(VK_F2) ? "On" : "OFF ");
	}
	ImGui::End();

	ImGui::BeginMainMenuBar();
	ImGui::Button("File");
	ImGui::EndMainMenuBar();

}

void App::OutoutSceneName() const
{
	std::stringstream ss;
	const std::string SceneName((*m_CurScene)->GetName().size() + 4, '*');

	ss << SceneName << std::endl
		<< "* " << (*m_CurScene)->GetName() << " *" << std::endl
		<< SceneName << std::endl;
	OutputDebugStringA(ss.str().c_str());
}

void App::CycleScenes()
{
	if (++m_CurScene == m_Scenes.end())
	{
		m_CurScene = m_Scenes.begin();
	}
	OutoutSceneName();
}

void App::RenderToTexture() 
{
	//SetRenderTarget();ClearDepth();
	m_DepthRT->SetRenderTarget(wnd.Gfx());
	//Render Depth Texture into mDepth .SRV
	RenderDepthTexture();
	
	//SetRenderTarget();ClearDepth();
	//m_SrceenRT->SetRenderTarget(wnd.Gfx());
	//RenderScene Save  into mScreenRTT .SRV
	//RenderScene();
	//wnd.Gfx().SetBackBufferRenderTarget();



	wnd.Gfx().SetBackBufferRenderTarget();
	wnd.Gfx().ResetViewport();
}

void App::RenderScene()
{

	// draw scene
	(*m_CurScene)->Draw();
}

void App::RenderDepthTexture()
{
	// draw scene
	(*m_CurScene)->DrawDepth();
}

void App::InitDebugWindow()
{
	//Screen Scene Small
	//m_SrceenRT = std::make_shared<Bind::RTT>(wnd.Gfx(), screenWidth, screenHeight);
	//m_SmallScene = std::make_unique<Tex2D>(wnd.Gfx(),
	//	static_cast<float>(screenWidth), static_cast<float>(screenHeight),
	//	static_cast<float>(screenWidth / 6),
	//	static_cast<float>(screenHeight / 6), "VS_2D", "PS_2D", m_SrceenRT->GetShaderResourceView());
	//m_SmallScene->SetPos({ 0.0f,screenHeight * 5.0f / 6.0f,0.0f });
	//shadow Map
	m_DepthRT = std::make_shared<Bind::DepthBufferRT>(wnd.Gfx(), shadowWidth, shadowHeight);
	m_ProjDepthMap = std::make_unique<Tex2D>(wnd.Gfx(), static_cast<float>(screenWidth), static_cast<float>(screenHeight),
		static_cast<float>(screenWidth / 6),
		static_cast<float>(screenHeight / 6), "VS_2D", "DepthDraw_PS", m_DepthRT->GetShaderResourceView());
	m_ProjDepthMap->SetPos({ 0,screenHeight * 5.0f / 6.0f,0.0f });
}







