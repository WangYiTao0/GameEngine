#pragma 
#include"GeometryHeader.h"

class Grid
{


public:

	static IndexedTriangleList MakeIndependent(Dvtx::VertexLayout layout,
		float width, float depth,
		unsigned int m, unsigned int n)
	{
		using Type = Dvtx::VertexLayout::ElementType;

		assert(m >= 2);
		assert(n >= 2);

		uint32 vertexCount = m * n;
		uint32 faceCount = (m - 1) * (n - 1) * 2;

		// Create the vertices.
		float halfWidth = 0.5f * width;
		float halfDepth = 0.5f * depth;

		float dx = width / (n - 1);
		float dz = depth / (m - 1);

		float du = 1.0f / (n - 1);
		float dv = 1.0f / (m - 1);

		//without 0
		Dvtx::VertexBuffer vb(std::move(layout),vertexCount);
		for (uint32 i = 0; i < m; ++i)
		{
			float z = halfDepth - i * dz;
			for (uint32 j = 0; j < n; ++j)
			{
				float x = -halfWidth + j * dx;
				//pos
				vb[i * n + j].Attr<Type::Position3D>() = DirectX::XMFLOAT3(x, 0.0f, z);
				//Normal
				//vb.EmplaceBack(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));
				////TangentU
				//vb.EmplaceBack(DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f));
				////textoord
				//vb.EmplaceBack(j * du);
				//vb.EmplaceBack(i * dv);
				//meshData.Vertices[i * n + j].Position = XMFLOAT3(x, 0.0f, z);
				//meshData.Vertices[i * n + j].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
				//meshData.Vertices[i * n + j].TangentU = XMFLOAT3(1.0f, 0.0f, 0.0f);
				// Stretch texture over grid.
				//meshData.Vertices[i * n + j].TexC.x = j * du;
				//meshData.Vertices[i * n + j].TexC.y = i * dv;
			}
		}

		// Create the indices.
		std::vector<unsigned short>indices;
		indices.resize(faceCount * 3);

		// Iterate over each quad and compute indices.
		uint32 k = 0;
		for (uint32 i = 0; i < m - 1; ++i)
		{
			for (uint32 j = 0; j < n - 1; ++j)
			{
				indices.push_back ( i * n + j);
				indices.push_back (i * n + j + 1);
				indices.push_back ( (i + 1) * n + j);
				
				indices.push_back ( (i + 1) * n + j);
				indices.push_back ( i * n + j + 1);
				indices.push_back ( (i + 1) * n + j + 1);
					
				k += 6; // next quad
			}
		}

		return { std::move(vb),std::move(indices) };

	}

	static IndexedTriangleList Make(std::optional<Dvtx::VertexLayout> layout = std::nullopt)
	{
		using Element = Dvtx::VertexLayout::ElementType;
		if (!layout)
		{
			layout = Dvtx::VertexLayout{}.Append(Element::Position3D);
		}
		return MakeIndependent(std::move(*layout), 24, 24, 12, 12);
	}
};




