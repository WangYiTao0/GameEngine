#pragma once
#include "Drawable.h"
#include "GameObject3D.h"

class SolidSphere : public Drawable,public GameObject3D
{
public:
	SolidSphere(Graphics& gfx, float radius);
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

};