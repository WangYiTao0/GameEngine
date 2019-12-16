#pragma once
#include "Scene.h"
#include "Graphics.h"
#include "Model.h"
#include "SkyRender.h"
#include "GridTerrain.h"

class PBRScene :public Scene
{
public:
	PBRScene(Graphics& gfx);
	~PBRScene();

	virtual void Update(float dt) override;
	virtual void Draw()override;
private:

	Graphics& gfx;
	Model pbrObject;	
	//GridTerrain gridTerrain{ gfx,160,160,50,50,20 };
	//std::unique_ptr<SkyRender> m_pSunset = nullptr;
};