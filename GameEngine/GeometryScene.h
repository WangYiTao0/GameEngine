#pragma once
#include "Scene.h"
#include "TestPlane.h"
#include "TestCube.h"
#include "SolidSphere.h"
#include "Collision.h"
#include <vector>
#include <memory>
#include "imgui/imgui.h"
#include "Frustum.h"
#include "GridTerrain.h"
#include "WaterWave.h"
#include "SkyRender.h"
#include "SponzaScene.h"
#include "FrameCommander.h"

class GeometryScene : public Scene
{
public:
	GeometryScene(Graphics& gfx);
	~GeometryScene();

	virtual void Update(float dt) override;

	virtual void Draw()  override;
	virtual void DrawDepth() override;
private:
	void SpownFrustumControl();
private:
	Graphics& gfx;
	std::unique_ptr<Light> m_Light;
	FrameCommander fc;

	bool EnableFrustumCulling = true;
	GridTerrain gridTerrain{ gfx,160,160,50,50,20 };

	std::vector<std::unique_ptr<TestCube>> cubes;
	std::mt19937 rng;
	std::unique_ptr<SkyRender> sky = nullptr;

};