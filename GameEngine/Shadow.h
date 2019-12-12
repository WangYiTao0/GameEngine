#pragma once
#include "Drawable.h"

class Shadow : public Drawable
{
public:
	Shadow(Graphics& gfx, std::string VS_name,std::string PS_name);
	DirectX::XMMATRIX GetTransformXM() const noexcept override;



};