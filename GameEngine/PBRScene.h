#pragma once
#include "Scene.h"
#include "Graphics.h"
#include "Model.h"
#include "SkyRender.h"
#include "GridTerrain.h"
#include "PBRBall.h"
#include "PBRLight.h"


class PBRScene :public Scene
{
public:
	PBRScene(Graphics& gfx);
	~PBRScene();

	virtual void Update(float dt) override;
	virtual void Draw()override;
private:

	Graphics& gfx;
	PBRLight light;
	std::vector<std::unique_ptr<PBRBall>> pbrBall;

	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;
};