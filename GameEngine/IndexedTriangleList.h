#pragma once
#include "Vertex.h"
#include <vector>
#include <DirectXMath.h>
#include "MathHelper.h"

class IndexedTriangleList
{
public:
	IndexedTriangleList() = default;
	IndexedTriangleList(Dvtx::VertexBuffer verts_in, std::vector<unsigned short>indices_in)
		:
		vertices(std::move(verts_in)),
		indices(std::move(indices_in))
	{
		assert(vertices.Size() > 2);
		assert(indices.size() % 3 == 0);
	}
	void Transform(DirectX::FXMMATRIX matrix)
	{
		using Elements = Dvtx::VertexLayout::ElementType;
		//for (auto& v : vertices)
		//{
		//	const DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&v.pos);
		//	DirectX::XMStoreFloat3(&v.pos,
		//		DirectX::XMVector3Transform(pos, matrix)
		//	);
		//}
		for( int i = 0; i < vertices.Size(); i++ )
		{
			auto& pos = vertices[i].Attr<Elements::Position3D>();
			DirectX::XMStoreFloat3(
				&pos,
				DirectX::XMVector3Transform( 
					DirectX::XMLoadFloat3( &pos ),matrix )
			);
		}
	}

	//// asserts face-independent vertices w/ normals cleared to zero
	void SetNormalsIndependentFlat() noxnd
	{
		using namespace DirectX;
		using Type = Dvtx::VertexLayout::ElementType;
		assert(indices.size() % 3 == 0 && indices.size() > 0);
		for (size_t i = 0; i < indices.size(); i += 3)
		{
			auto v0 = vertices[indices[i]];
			auto v1 = vertices[indices[i + 1]];
			auto v2 = vertices[indices[i + 2]];
			const auto p0 = XMLoadFloat3(&v0.Attr<Type::Position3D>());
			const auto p1 = XMLoadFloat3(&v1.Attr<Type::Position3D>());
			const auto p2 = XMLoadFloat3(&v2.Attr<Type::Position3D>());

			const auto n = XMVector3Normalize(XMVector3Cross((p1 - p0), (p2 - p0)));
			
			XMStoreFloat3(&v0.Attr<Type::Normal>(), n);
			XMStoreFloat3(&v1.Attr<Type::Normal>(), n);
			XMStoreFloat3(&v2.Attr<Type::Normal>(), n);
		}
	}

	void ComputeTangentBiTtngent() noxnd
	{
		using namespace DirectX;
		using Type = Dvtx::VertexLayout::ElementType;
		for (size_t i = 0; i < indices.size(); i += 3)
		{
			auto v0 = vertices[indices[i]];
			auto v1 = vertices[indices[i + 1]];
			auto v2 = vertices[indices[i + 2]];

			// Shortcuts for vertices
			const auto p0 = XMLoadFloat3(&v0.Attr<Type::Position3D>());
			const auto p1 = XMLoadFloat3(&v1.Attr<Type::Position3D>());
			const auto p2 = XMLoadFloat3(&v2.Attr<Type::Position3D>());

			// Shortcuts for UVs
			const auto uv0 = XMLoadFloat2(&v0.Attr<Type::Texture2D>());
			const auto uv1 = XMLoadFloat2(&v1.Attr<Type::Texture2D>());
			const auto uv2 = XMLoadFloat2(&v2.Attr<Type::Texture2D>());

			// Edges of the triangle : postion delta
			auto deltaPos1 = p1 - p0;
			auto deltaPos2 = p2 - p0;

			// UV delta
			auto deltaUV1 = uv1 - uv0;
			auto deltaUV2 = uv2 - uv0;

			DirectX::XMFLOAT3 fdeltaPos1, fdeltaPos2;
			DirectX::XMFLOAT2 fdeltaUV1, fdeltaUV2;

			DirectX::XMStoreFloat3(&fdeltaPos1, deltaPos1);
			DirectX::XMStoreFloat3(&fdeltaPos2, deltaPos2);

			DirectX::XMStoreFloat2(&fdeltaUV1, deltaUV1);
			DirectX::XMStoreFloat2(&fdeltaUV2, deltaUV2);
	
			float r = 1.0f / (fdeltaUV1.x * fdeltaUV2.y - fdeltaUV2.x * fdeltaUV1.y);
			auto tangent = (deltaPos1 * fdeltaUV2.y - deltaPos2 * fdeltaUV1.y) * r;
			auto bitangent = (deltaPos2 * fdeltaUV1.x - deltaPos1 * fdeltaUV2.x) * r;
			tangent = DirectX::XMVector3Normalize(tangent);
			bitangent = DirectX::XMVector3Normalize(bitangent);
			XMStoreFloat3(&v0.Attr<Type::Tangent>(), tangent);
			XMStoreFloat3(&v1.Attr<Type::Tangent>(), tangent);
			XMStoreFloat3(&v2.Attr<Type::Tangent>(), tangent);
			XMStoreFloat3(&v0.Attr<Type::Bitangent>(), bitangent);
			XMStoreFloat3(&v1.Attr<Type::Bitangent>(), bitangent);
			XMStoreFloat3(&v2.Attr<Type::Bitangent>(), bitangent);

		}
	}


	void ComputeSphereUV() noxnd
	{
		using namespace DirectX;
		using Type = Dvtx::VertexLayout::ElementType;
		for (int i = 0; i < vertices.Size(); i++)
		{
			auto v0 = vertices[i];
			// Shortcuts for vertices
			const auto p0 = v0.Attr<Type::Position3D>();
			
			float u = std::asin(p0.z / 1.0) / MH::PI + 0.5f;
			float v = std::atan(p0.y / p0.x) / 2 / MH::PI;

			v0.Attr<Type::Texture2D>() = { u,v };

		}
	}

public:
	Dvtx::VertexBuffer vertices;
	std::vector<unsigned short> indices;
};