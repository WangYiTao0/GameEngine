#pragma once
#include "ConstantBuffers.h"
#include "Drawable.h"
#include <DirectXMath.h>
namespace Bind
{
	class TransformCbuf : public Bindable
	{
	protected:
		struct Transforms
		{
			DirectX::XMMATRIX world;
			DirectX::XMMATRIX worldInvTanspose;
			//camera view pos
			DirectX::XMMATRIX view;
			DirectX::XMMATRIX viewInv;

			//screen pos
			DirectX::XMMATRIX proj;
			//Camera2D
			DirectX::XMMATRIX ortho;
			DirectX::XMMATRIX world2D;	

			DirectX::XMFLOAT3 camera3DPos;
			float Padding;
		};
	public:
		TransformCbuf(Graphics& gfx, UINT slot = 0u);
		void Bind(Graphics& gfx) noexcept override;
		void InitializeParentReference(const Drawable& parent) noexcept override;
	protected:
		void UpdateBindImpl(Graphics& gfx, const Transforms& tf)noexcept;
		Transforms GetTransforms(Graphics& gfx) noexcept;
	private:
		static std::unique_ptr<VertexConstantBuffer<Transforms>> pVcbuf;
		const Drawable* pParent = nullptr;
	};
}