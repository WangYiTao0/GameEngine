#pragma once
#include "Scene.h"
#include "Graphics.h"
#include "Model.h"
#include "SkyRender.h"
#include "GridTerrain.h"
#include "PBRBall.h"
#include "PBRLight.h"
#include "SkyRender.h"
#include "Light.h"

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
	//Light
	std::unique_ptr<Light> m_Light;
	std::vector<std::unique_ptr<PBRBall>> pbrBall;

	std::unique_ptr<SkyRender> skyHdr;

	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;
};