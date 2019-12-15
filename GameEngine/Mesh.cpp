#include "Mesh.h"

Mesh::Mesh(Graphics& gfx, std::vector<std::shared_ptr<Bind::Bindable>> bindPtrs)
{
	AddBind(Bind::Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	for (auto& pb : bindPtrs)
	{
		AddBind(std::move(pb));
	}
	AddBind(std::make_shared<Bind::TransformVertexAndPixelCbuf>(gfx, *this, 0u, 0u));
}

void Mesh::Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept(!IS_DEBUG)
{
	DirectX::XMStoreFloat4x4(&transform, accumulatedTransform);
	Drawable::DrawIndexed(gfx);
}


DirectX::XMMATRIX Mesh::GetTransformXM() const noexcept
{
	return DirectX::XMLoadFloat4x4(&transform);
}
