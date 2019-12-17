#pragma once

#include "Drawable.h"
#include "GameObject.h"

class PBRBall :public Drawable, public GameObject
{
public:
	PBRBall(Graphics& gfx, float radius);
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
};