#pragma once
#include "Scene.h"
#include "Model.h"
#include "TestPlane.h"
#include "SkyRender.h"


class SponzaScene : public Scene
{
public:
	SponzaScene(Graphics& gfx);

	virtual void Update(float dt) override;

	virtual void Draw()override;
private:
	Graphics& gfx;

	std::unique_ptr<Model> sponza;

	std::unique_ptr<SkyRender> m_pSunset = nullptr;

	TestPlane bluePlane{ gfx,6.0f,{ 0.3f,0.3f,1.0f,0.0f } };
	TestPlane redPlane{ gfx,6.0f,{ 1.0f,0.3f,0.3f,0.0f } };
};