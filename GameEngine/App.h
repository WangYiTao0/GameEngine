#pragma once
#include "Window.h"
#include "GameTimer.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include "DirectionLight.h"
#include "BoundingSphere.h"
#include "IntersectData.h"
#include <set>
#include "Scene.h"
#include "NormalMapTwerker.h"
#include <shellapi.h>
#include "CpuClass.h"


class App
{
private:
	enum class LightType
	{
		DirectionLightType = 0,
		PointLightType,
		SpotLightType,

		MaxType
	} lightType;

public:
	App(const std::string& commandLine = "");
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
private:
	std::string commandLine;
	bool showDemoWindow = false;
	int x = 0, y = 0;
	ImguiManager imgui;
	Window wnd;
	GameTimer timer;
	float speed_factor = 1.0f;
	Camera cam;
	PointLight pointLight;
	DirectionLight directionLight;

	
	CpuClass m_Cpu;


	std::vector <std::unique_ptr<Scene>> scenes;
	std::vector <std::unique_ptr<Scene>>::iterator curScene;
};