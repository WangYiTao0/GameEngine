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
#include "Tex2D.h"




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

	Tex2D tex2D { gfx,512,512,"Data\\Images\\spnza_bricks_a_diff.png"};
	GridTerrain gridTerrain{ gfx,160,160,50,50,20 };
	TestCube cube{ gfx,2};

};