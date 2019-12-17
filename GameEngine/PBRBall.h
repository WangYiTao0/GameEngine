#pragma once

#include "Drawable.h"
#include "GameObject.h"

class PBRBall :public Drawable, public GameObject
{
public:
	PBRBall(Graphics& gfx, float radius);
	void AddPBRTexture(Graphics& gfx,std::vector<std::string>& paths);
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
};