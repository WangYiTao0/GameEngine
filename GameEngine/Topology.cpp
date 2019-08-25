#include "Topology.h"
namespace Bind
{
	Topology::Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type)
		:
		type(type)
	{
	}

	void Topology::Bind(Graphics& gfx) noexcept
	{
		//IA input-Assember
		GetContext(gfx)->IASetPrimitiveTopology(type);

	}
}
