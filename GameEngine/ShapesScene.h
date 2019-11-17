#pragma once
#include "Scene.h"
#include "Graphics.h"
#include "GridTerrain.h"
#include "Mesh.h"


class ShapesScene :public Scene
{
public:
	ShapesScene(Graphics& gfx);
	virtual void Update(float dt) override;
	virtual void Draw()  override;
private:
	Graphics& gfx;
	GridTerrain gridTerrain;
	Model armory_Blast_W_HIGH{gfx,"Data\\Models\\armory_Blast_W_HIGH\\armory_Blast_W_HIGH.fbx",5.0f};
	//Model Samba{ gfx,"Data\\Models\\mixamo\\SambaDancing2.fbx",100.0f };
};