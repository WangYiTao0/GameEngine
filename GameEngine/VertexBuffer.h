#pragma once
#include "Bindable.h"
#include "GraphicsThrowMacros.h"
#include "Vertex.h"
namespace Bind
{
	class VertexBuffer : public Bindable
	{
	public:
		VertexBuffer(Graphics& gfx,  const std::string& tag, const Dvtx::VertexBuffer& vbuf);
		VertexBuffer(Graphics& gfx, const Dvtx::VertexBuffer& vbuf);
		VertexBuffer(Graphics& gfx, const std::string& tag, const Dvtx::VertexBuffer& vbuf, int usage );
		void Bind(Graphics& gfx)noexcept override;
		const Dvtx::VertexLayout& GetLayout() const noexcept;

		template<typename T>
		void UpdateDynamicVertexBuffer(Graphics& gfx, T& vs)
		{
			INFOMAN(gfx);

			D3D11_MAPPED_SUBRESOURCE mappedResource;
			GFX_THROW_INFO(GetContext(gfx)->Map(
				pVertexBuffer.Get(), 0u,
				D3D11_MAP_WRITE_DISCARD, 0u,
				&mappedResource));

			T* vsPtr;
			vsPtr = (T*)mappedResource.pData;

			memcpy(vsPtr, &vs, sizeof(vs));
			GetContext(gfx)->Unmap(pVertexBuffer.Get(), 0u);

		}

		static std::shared_ptr<VertexBuffer> Resolve(Graphics& gfx, const std::string& tag,
			const Dvtx::VertexBuffer& vbuf,int usage = D3D11_USAGE::D3D11_USAGE_DEFAULT);
		template<typename...Ignore>
		static std::string GenerateUID(const std::string& tag, Ignore&& ...ignore)
		{
			return GenerateUID_(tag);
		}
		std::string GetUID() const noexcept override;
	private:
		static std::string GenerateUID_(const std::string& tag);
	protected:
		std::string tag;
		UINT stride;
		int usage;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
		Dvtx::VertexLayout layout;
	};
}