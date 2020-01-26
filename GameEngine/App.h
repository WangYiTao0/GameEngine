#pragma once
#include "Window.h"
#include "GameTimer.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "Light.h"
#include "BoundingSphere.h"
#include "IntersectData.h"
#include <set>
#include "Scene.h"
#include <shellapi.h>
#include "CpuClass.h"
#include "Tex2D.h"
#include "RTT.h"
#include "DepthBufferRT.h"
#include "FrameCommander.h"

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
	void RenderDepthTexture();
	void InitDebugWindow();

private:
	const int screenWidth = 1600;
	const int screenHeight = 900;
	const float nearZ = 0.1f;
	const float farZ = 4000.0f;

	const int shadowWidth = 1024;
	const int shadowHeight = 1024;

	bool showDemoWindow = false;
	int x = 0, y = 0;
	ImguiManager imgui;
	Window wnd;

	GameTimer timer;
	float speed_factor = 1.0f;

	//Camera3D cam;
	Camera2D m_Camera2D;

	FrameCommander m_fc;

	CpuClass m_Cpu;

	std::vector <std::unique_ptr<Scene>> m_Scenes;
	std::vector <std::unique_ptr<Scene>>::iterator m_CurScene;


	bool enableRenderTarget = false;
	std::shared_ptr<Bind::RTT> m_SrceenRT;


	//
	std::unique_ptr <Tex2D> m_SmallScene;
	std::unique_ptr <Tex2D> m_ProjDepthMap;
public:
	static std::shared_ptr<Bind::DepthBufferRT> m_DepthRT;
};