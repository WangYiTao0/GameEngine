#pragma once

#include "GeometryHeader.h"

class Sphere
{
public:

	static IndexedTriangleList MakeTesselated(Dvtx::VertexLayout layout, int latDiv, int longDiv)
	{
		namespace dx = DirectX;
		assert(latDiv >= 3);
		assert(longDiv >= 3);

		constexpr float radius = 1.0f;
		const auto base = dx::XMVectorSet(0.0f, 0.0f, radius, 0.0f);
		const float lattitudeAngle = MH::PI / latDiv;
		const float longitudeAngle = 2.0f * MH::PI / longDiv;

		Dvtx::VertexBuffer vb{ std::move(layout) };
		for (int iLat = 1; iLat < latDiv; iLat++)
		{
			const auto latBase = dx::XMVector3Transform(
				base,
				dx::XMMatrixRotationX(lattitudeAngle * iLat)
			);
			for (int iLong = 0; iLong < longDiv; iLong++)
			{
				dx::XMFLOAT3 calculatedPos;
				auto v = dx::XMVector3Transform(
					latBase,
					dx::XMMatrixRotationZ(longitudeAngle * iLong)
				);
				dx::XMStoreFloat3(&calculatedPos, v);
				vb.EmplaceBack(calculatedPos);
			}
		}

		// add the cap vertices
		const auto iNorthPole = (unsigned short)vb.Size();
		{
			dx::XMFLOAT3 northPos;
			dx::XMStoreFloat3(&northPos, base);
			vb.EmplaceBack(northPos);
		}
		const auto iSouthPole = (unsigned short)vb.Size();
		{
			dx::XMFLOAT3 southPos;
			dx::XMStoreFloat3(&southPos, dx::XMVectorNegate(base));
			vb.EmplaceBack(southPos);
		}

		const auto calcIdx = [latDiv, longDiv](unsigned short iLat, unsigned short iLong)
		{ return iLat * longDiv + iLong; };
		std::vector<unsigned short> indices;
		for (unsigned short iLat = 0; iLat < latDiv - 2; iLat++)
		{
			for (unsigned short iLong = 0; iLong < longDiv - 1; iLong++)
			{
				indices.push_back(calcIdx(iLat, iLong));
				indices.push_back(calcIdx(iLat + 1, iLong));
				indices.push_back(calcIdx(iLat, iLong + 1));
				indices.push_back(calcIdx(iLat, iLong + 1));
				indices.push_back(calcIdx(iLat + 1, iLong));
				indices.push_back(calcIdx(iLat + 1, iLong + 1));
			}
			// wrap band
			indices.push_back(calcIdx(iLat, longDiv - 1));
			indices.push_back(calcIdx(iLat + 1, longDiv - 1));
			indices.push_back(calcIdx(iLat, 0));
			indices.push_back(calcIdx(iLat, 0));
			indices.push_back(calcIdx(iLat + 1, longDiv - 1));
			indices.push_back(calcIdx(iLat + 1, 0));
		}

		// cap fans
		for (unsigned short iLong = 0; iLong < longDiv - 1; iLong++)
		{
			// north
			indices.push_back(iNorthPole);
			indices.push_back(calcIdx(0, iLong));
			indices.push_back(calcIdx(0, iLong + 1));
			// south
			indices.push_back(calcIdx(latDiv - 2, iLong + 1));
			indices.push_back(calcIdx(latDiv - 2, iLong));
			indices.push_back(iSouthPole);
		}
		// wrap triangles
		// north
		indices.push_back(iNorthPole);
		indices.push_back(calcIdx(0, longDiv - 1));
		indices.push_back(calcIdx(0, 0));
		// south
		indices.push_back(calcIdx(latDiv - 2, 0));
		indices.push_back(calcIdx(latDiv - 2, longDiv - 1));
		indices.push_back(iSouthPole);

		return { std::move(vb),std::move(indices) };
	}

	static IndexedTriangleList MakeSphereTexture(Dvtx::VertexLayout layout,
		int latDiv, int longDiv)
	{
		namespace dx = DirectX;
		assert(latDiv >= 3);
		assert(longDiv >= 3);

		constexpr float radius = 1.0f;
		const auto base = dx::XMVectorSet(0.0f, 0.0f, radius, 0.0f);
		const float lattitudeAngle = MH::PI / latDiv;
		const float longitudeAngle = 2.0f * MH::PI / longDiv;

		//without 0
		Dvtx::VertexBuffer vb(std::move(layout));

			//add the cap vert
		//store last two point in the back of vertor<>
		const auto iNorthPole = (unsigned short)vb.Size();
		{
			dx::XMFLOAT3 northPos;
			dx::XMStoreFloat3(&northPos, base);
			//vb.EmplaceBack(northPos);
			vb.EmplaceBack(northPos,
				northPos,
				northPos,
				northPos,
				DirectX::XMFLOAT2(1.0f, 1.0f));
		}

		for (int iLat = 1; iLat < latDiv; iLat++)
		{
			const auto latBase = dx::XMVector3Transform(
				base,
				dx::XMMatrixRotationX(lattitudeAngle * iLat)
				);
			for (int iLong = 0; iLong < longDiv; iLong++)
			{
				dx::XMFLOAT3  calculatedPos;
				auto v = dx::XMVector3Transform(
					latBase,
					dx::XMMatrixRotationZ(longitudeAngle * iLong)
				);
				dx::XMStoreFloat3(&calculatedPos, v);
				vb.EmplaceBack(calculatedPos,
					calculatedPos,
					calculatedPos,
					calculatedPos,
					DirectX::XMFLOAT2(1.0f, 1.0f));
			}
		}

		//add the cap vert
		////store last two point in the back of vertor<>
		//const auto iNorthPole = (unsigned short)vb.Size();
		//{
		//	dx::XMFLOAT3 northPos;
		//	dx::XMStoreFloat3(&northPos, base);
		//	//vb.EmplaceBack(northPos);
		//	vb.EmplaceBack(northPos,
		//		northPos,
		//		northPos,
		//		northPos,
		//		DirectX::XMFLOAT2(1.0f, 1.0f));
		//}
		const auto iSouthPole = (unsigned short)vb.Size();
		{
			dx::XMFLOAT3 southPos;
			dx::XMStoreFloat3(&southPos, dx::XMVectorNegate(base));
			//vb.EmplaceBack(southPos);
			vb.EmplaceBack(southPos,
				southPos,
				southPos,
				southPos,
				DirectX::XMFLOAT2(1.0f, 1.0f));
		}
		//[capture](paramter) mutable throw() -> int 
		//{  }
		//[=] capture everything by value
		//[&] capture everything by reference
		//[this] capture member function
		// + mutable to change value in lambda in{}
		const auto calcIdx = [latDiv, longDiv]
		(unsigned short iLat, unsigned short iLong)
		{ return iLat * longDiv + iLong; };
		std::vector<unsigned short> indices;
		//calculate index
		for (unsigned short iLat = 0; iLat < latDiv - 2; iLat++)
		{
			for (unsigned short iLong = 0; iLong < longDiv - 1; iLong++)
			{
				indices.push_back(calcIdx(iLat, iLong)); //0,0
				indices.push_back(calcIdx(iLat + 1, iLong));//1,0
				indices.push_back(calcIdx(iLat, iLong + 1));//0,1
				indices.push_back(calcIdx(iLat, iLong + 1));//0,1
				indices.push_back(calcIdx(iLat + 1, iLong));//1,0
				indices.push_back(calcIdx(iLat + 1, iLong + 1));//1,1
			}
			// wrap band 
			indices.push_back(calcIdx(iLat, longDiv - 1)); //0,0
			indices.push_back(calcIdx(iLat + 1, longDiv - 1));//1,0
			indices.push_back(calcIdx(iLat, 0));//0,1
			indices.push_back(calcIdx(iLat, 0));//0,1
			indices.push_back(calcIdx(iLat + 1, longDiv - 1));//1,0
			indices.push_back(calcIdx(iLat + 1, 0));//1,1
		}


		// cap fans
		for (unsigned short iLong = 0; iLong < longDiv - 1; iLong++)
		{
			// north
			indices.push_back(iNorthPole);
			indices.push_back(calcIdx(0, iLong));
			indices.push_back(calcIdx(0, iLong + 1));
			// south
			indices.push_back(calcIdx(latDiv - 2, iLong + 1));
			indices.push_back(calcIdx(latDiv - 2, iLong));
			indices.push_back(iSouthPole);
		}
		//special 
		// wrap triangles
		// north
		indices.push_back(iNorthPole);
		indices.push_back(calcIdx(0, longDiv - 1));
		indices.push_back(calcIdx(0, 0));
		// south
		indices.push_back(calcIdx(latDiv - 2, 0));
		indices.push_back(calcIdx(latDiv - 2, longDiv - 1));
		indices.push_back(iSouthPole);

		return { std::move(vb),std::move(indices) };
	}

	static IndexedTriangleList MakeUVSphere(Dvtx::VertexLayout layout,
		int X_SEGMENTS = 64, int Y_SEGMENTS = 64)
	{

		Dvtx::VertexBuffer vb{ std::move(layout) };
		const float PI = 3.14159265359;
		for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
		{
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
			{
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

				vb.EmplaceBack(
					DirectX::XMFLOAT3(xPos, yPos, zPos),
					DirectX::XMFLOAT3(xPos, yPos, zPos),
					DirectX::XMFLOAT3(xPos, yPos, zPos),
					DirectX::XMFLOAT3(xPos, yPos, zPos),
					DirectX::XMFLOAT2(xSegment, ySegment));
			}
		}

		std::vector<unsigned short> indices;
		bool oddRow = false;
		for (int y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow) // even rows: y == 0, y == 2; and so on
			{
				for (int x = 0; x <= X_SEGMENTS; ++x)
				{
					indices.push_back(y * (X_SEGMENTS + 1) + x);
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			else
			{
				for (int x = X_SEGMENTS; x >= 0; --x)
				{
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					indices.push_back(y * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
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
		return MakeTesselated(std::move(*layout), 12, 24);
	}
};