#pragma once
#include "Window.h"
#include "GameTimer.h"

#include "ImguiManager.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

#include "Camera.h"
#include "LightCommon.h"
#include "BoundingSphere.h"
#include "IntersectData.h"
#include <set>
#include "Scene.h"
#include <shellapi.h>
#include "CpuClass.h"
#include "Tex2D.h"
#include "RTT.h"

class App
{

public:
	App();
	//master frame / message loop
	int Go();
	~App();
private:
	void HandleInput(float dt);
	void update(float dt);
	void Draw();
	void DoFrame();
	void SpawnEngineStateWindow();
	void OutoutSceneName()const;
	void CycleScenes();

	void RenderToTexture();
	void RenderScene();

private:

	const int width = 1600;
	const int height = 900;
	const float nearZ = 0.1f;
	const float farZ = 4000.0f;

	bool showDemoWindow = false;
	int x = 0, y = 0;
	ImguiManager imgui;
	Window wnd;

	GameTimer timer;

	float speed_factor = 1.0f;
	Camera3D cam;
	Camera2D cam2D;

	PointLight pointLight;

	CpuClass m_Cpu;

	std::vector <std::unique_ptr<Scene>> scenes;
	std::vector <std::unique_ptr<Scene>>::iterator curScene;

	Bind::RTT rtt;
	//
	Tex2D tex2{ wnd.Gfx(),width,height,480,270,rtt.GetShaderResourceView() };

};