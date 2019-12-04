#pragma once
#include "Scene.h"
#include "Graphics.h"
#include "Mesh.h"

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
};