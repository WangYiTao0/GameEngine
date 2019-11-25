#pragma once
#include "Drawable.h"
#include "GameObject.h"

class SolidSphere : public Drawable,public GameObject
{
public:
	SolidSphere(Graphics& gfx, float radius);
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

};