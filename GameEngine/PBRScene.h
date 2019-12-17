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
#include "Model.h"

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
	//std::vector<std::unique_ptr<PBRBall>> pbrBall;

	std::unique_ptr<SkyRender> skyHdr;
	std::vector<std::unique_ptr<Model>> shaderBall;

	int nrRows = 3;
	int nrColumns = 3;
	float spacing = 5.0f;
};