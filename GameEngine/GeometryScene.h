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
	TestCube cube{ gfx,2};

};