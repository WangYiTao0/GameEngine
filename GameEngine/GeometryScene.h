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

	std::unique_ptr<Frustum> frustum;
	bool EnableFrustumCulling = true;
	bool isVisiable = false;

	TestPlane plane;

	std::vector<std::unique_ptr<class TestCube>> cubes;
	std::vector<DirectX::XMMATRIX> m_InstancedData;
	std::vector<DirectX::XMMATRIX> refData;
	//Collision::WireFrameData BoxData;

	std::mt19937 rng;
	const int objectCount = 180;
	int renderCount = 0;
	SolidSphere sphere;
};