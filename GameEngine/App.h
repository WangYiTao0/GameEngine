#pragma once
#include "Window.h"
#include "ChiliTimer.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include "BoundingSphere.h"
#include "IntersectData.h"
#include <set>
#include "Scene.h"



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
	void ShowImguiDemoWindow();
	void OutoutSceneName()const;
	void CycleScenes();
private:
	bool showDemoWindow = false;
	int x = 0, y = 0;
	ImguiManager imgui;
	Window wnd;
	ChiliTimer timer;
	float speed_factor = 1.0f;
	Camera cam;
	PointLight light;

	BoundingSphere 	bs1;
	BoundingSphere bs2;

	std::vector <std::unique_ptr<Scene>> scenes;
	std::vector <std::unique_ptr<Scene>>::iterator curScene;
};