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

	

	TestPlane bluePlane{ gfx,6.0f,{ 0.3f,0.3f,1.0f,0.0f } };
	TestPlane redPlane{ gfx,6.0f,{ 1.0f,0.3f,0.3f,0.0f } };
	GridTerrain gridTerrain{ gfx,160,160,50,50 };
	std::vector<std::unique_ptr<class TestCube>> cubes;
	//std::vector<TestCube> cubes;

	std::vector<DirectX::XMMATRIX> m_InstancedData;
	std::vector <DirectX::BoundingBox> BoxData;
	std::vector<DirectX::XMMATRIX> refData;
	//Collision::WireFrameData BoxData;

	std::mt19937 rng;
	const int objectCount = 180;
	int renderCount = 0;
	SolidSphere sphere;
};