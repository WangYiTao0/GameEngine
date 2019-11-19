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
	Model Role{gfx,"Data\\Models\\Role\\Role_01\\FBX\\Role.FBX",50.0f};
	//Model Samba{ gfx,"Data\\Models\\mixamo\\SambaDancing2.fbx",100.0f };
};