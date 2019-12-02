#pragma once
#include "Drawable.h"
#include "GameObject3D.h"
#include "Graphics.h"

class PlaneMirror : public Drawable,public GameObject3D
{
public:
	PlaneMirror(Graphics& gfx);

	DirectX::XMMATRIX GetTransformXM()const noexcept override;

};