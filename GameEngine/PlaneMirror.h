#pragma once
#include "Drawable.h"
#include "GameObject.h"
#include "Graphics.h"

class PlaneMirror : public Drawable,public GameObject
{
public:
	PlaneMirror(Graphics& gfx, float size);

	DirectX::XMMATRIX GetTransformXM()const noexcept override;

};