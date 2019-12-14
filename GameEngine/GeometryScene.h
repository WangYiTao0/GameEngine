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
#include "ModelScene.h"

class GeometryScene : public Scene
{
public:
	GeometryScene(Graphics& gfx);

	virtual void Update(float dt) override;

	virtual void Draw()  override;
private:
	void SpownFrustumControl();
private:
	Graphics& gfx;

	bool EnableFrustumCulling = true;
	GridTerrain gridTerrain{ gfx,160,160,50,50,20 };

	std::unique_ptr<Model> role;
	TestCube cube{ gfx,2};

	std::unique_ptr<SkyRender> m_pSunset = nullptr;

};