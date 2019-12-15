#pragma once
#include "Drawable.h"
#include "BindableCommon.h"

class Mesh : public Drawable
{
public:
	Mesh(Graphics& gfx, std::vector<std::shared_ptr<Bind::Bindable>> bindPtrs);
	void Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noxnd;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
	mutable DirectX::XMFLOAT4X4 transform;
};