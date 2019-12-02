#pragma once
#include "Drawable.h"
#include "GameObject2D.h"
class Tex2D :public Drawable ,public GameObject2D
{
public:
	Tex2D(Graphics& gfx, float width, float height, std::string filePath);
	DirectX::XMMATRIX GetTransformXM() const noexcept;
};