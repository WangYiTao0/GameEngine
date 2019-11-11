#pragma once
#include "Vertex.h"
#include "IndexedTriangleList.h"

class Frustum
{
public:
	Frustum() = default;

public:
	void BuildFrustum(float screenDepth, DirectX::XMMATRIX projectionMatrix, DirectX::XMMATRIX viewMatrix);

	bool CheckPoint(float x, float y, float z);


	bool CheckCube(float xCenter, float yCenter, float zCenter, float radius);


	bool CheckSphere(float xCenter, float yCenter, float zCenter, float radius);

	bool CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize);

private:
	DirectX::XMVECTOR m_planes[6] = { 0 };

};