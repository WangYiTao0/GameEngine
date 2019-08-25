#pragma once
#include "Bindable.h"
#include "GraphicsThrowMacros.h"
#include "Vertex.h"
namespace Bind
{
	class VertexBuffer : public Bindable
	{
	public:
		template<class V>
		VertexBuffer(Graphics& gfx, const std::vector<V>& vertices)
			:
			stride(sizeof(V))
		{
			INFOMAN(gfx);

			D3D11_BUFFER_DESC vbd = {};
			vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vbd.Usage = D3D11_USAGE_DEFAULT;
			vbd.CPUAccessFlags = 0u;
			vbd.MiscFlags = 0u;
			vbd.ByteWidth = UINT(sizeof(V) * vertices.size());
			vbd.StructureByteStride = sizeof(V);
			D3D11_SUBRESOURCE_DATA vsd = {};
			vsd.pSysMem = vertices.data();
			GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&vbd, &vsd, &pVertexBuffer));
		}
		VertexBuffer(Graphics& gfx, const Dvtx::VertexBuffer& vbuf);
		void Bind(Graphics& gfx)noexcept override;
	private:
		UINT stride;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	};
}