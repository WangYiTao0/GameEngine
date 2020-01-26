#include "VertexBuffer.h"
#include "BindableCodex.h"

namespace Bind
{

	VertexBuffer::VertexBuffer(Graphics& gfx, const Dvtx::VertexBuffer& vbuf)
		:
		VertexBuffer(gfx, "?", vbuf)
	{}
	VertexBuffer::VertexBuffer(Graphics& gfx, const std::string& tag, const Dvtx::VertexBuffer& vbuf)
		:
		VertexBuffer(gfx, tag, vbuf, D3D11_USAGE::D3D11_USAGE_DEFAULT)
	{

	}
	VertexBuffer::VertexBuffer(Graphics & gfx, const std::string & tag, const Dvtx::VertexBuffer & vbuf, int usage)
		:
		stride((UINT)vbuf.GetLayout().Size()),
		tag(tag),
		usage(usage),
		layout(vbuf.GetLayout())
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC bd = {};
		//bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		if (usage == (int)D3D11_USAGE::D3D11_USAGE_DYNAMIC)
		{
			bd.Usage = (D3D11_USAGE)usage;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		}
		else
		{
			bd.Usage = (D3D11_USAGE)usage;
			bd.CPUAccessFlags = 0u;
		}
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(vbuf.SizeBytes());
		bd.StructureByteStride = stride;
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vbuf.GetData();
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer));
	}


	void VertexBuffer::Bind(Graphics& gfx) noexcept
	{
		const UINT offset = 0;
		GetContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::Resolve(Graphics& gfx, 
		const std::string& tag,
		const Dvtx::VertexBuffer& vbuf,int usage)
	{
		assert(tag != "?");
		return Codex::Resolve<VertexBuffer>(gfx,tag,vbuf,usage);
	}
	std::string VertexBuffer::GenerateUID_(const std::string& tag)
	{
		using namespace std::string_literals;
		return typeid(VertexBuffer).name() + "#"s + tag;
	}
	std::string VertexBuffer::GetUID() const noexcept
	{
		return GenerateUID(tag);
	}
}
