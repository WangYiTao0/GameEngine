#include "Frustum.h"

void Frustum::BuildFrustum(float screenDepth, DirectX::XMMATRIX projectionMatrix, DirectX::XMMATRIX viewMatrix)
{
	float zMinimum, r;
	DirectX::XMMATRIX m_finalMatrix;

	DirectX::XMMATRIX m_projectionMatrix = projectionMatrix;
	DirectX::XMMATRIX m_viewMatrix = viewMatrix;

	DirectX::XMFLOAT4X4 projectionFloat4x4;
	DirectX::XMStoreFloat4x4(&projectionFloat4x4, m_projectionMatrix);
	DirectX::XMFLOAT4X4 viewFloat4x4;
	DirectX::XMStoreFloat4x4(&viewFloat4x4, m_viewMatrix);

	// Calculate the minimum Z distance in the frustum.
	zMinimum = -projectionFloat4x4._43 / projectionFloat4x4._33;
	r = screenDepth / (screenDepth - zMinimum);
	projectionFloat4x4._33 = r;
	projectionFloat4x4._43 = -r * zMinimum;

	m_projectionMatrix = DirectX::XMLoadFloat4x4(&projectionFloat4x4);
	m_viewMatrix = DirectX::XMLoadFloat4x4(&viewFloat4x4);
	//m_finalMatrix = DirectX::XMLoadFloat4x4(&finalFloat4x4);

	// Create the frustum matrix from the view matrix and updated projection matrix.
	m_finalMatrix = DirectX::XMMatrixMultiply(m_viewMatrix, m_projectionMatrix);

	DirectX::XMFLOAT4X4 finalFloat4x4;
	DirectX::XMStoreFloat4x4(&finalFloat4x4, m_finalMatrix);

	// Calculate near plane of frustum.
	DirectX::XMFLOAT4 nearPlane;
	nearPlane.x = finalFloat4x4._14 + finalFloat4x4._13;
	nearPlane.y = finalFloat4x4._24 + finalFloat4x4._23;
	nearPlane.z = finalFloat4x4._34 + finalFloat4x4._33;
	nearPlane.w = finalFloat4x4._44 + finalFloat4x4._43;
	m_planes[0] = DirectX::XMLoadFloat4(&nearPlane);
	m_planes[0] = DirectX::XMPlaneNormalize(m_planes[0]);

	// Calculate far plane of frustum.
	DirectX::XMFLOAT4 farPlane;
	farPlane.x = finalFloat4x4._14 - finalFloat4x4._13;
	farPlane.y = finalFloat4x4._24 - finalFloat4x4._23;
	farPlane.z = finalFloat4x4._34 - finalFloat4x4._33;
	farPlane.w = finalFloat4x4._44 - finalFloat4x4._43;
	m_planes[1] = DirectX::XMLoadFloat4(&farPlane);
	m_planes[1] = DirectX::XMPlaneNormalize(m_planes[1]);

	// Calculate left plane of frustum.
	DirectX::XMFLOAT4 leftPlane;
	leftPlane.x = finalFloat4x4._14 + finalFloat4x4._11;
	leftPlane.y = finalFloat4x4._24 + finalFloat4x4._21;
	leftPlane.z = finalFloat4x4._34 + finalFloat4x4._31;
	leftPlane.w = finalFloat4x4._44 + finalFloat4x4._41;
	m_planes[2] = DirectX::XMLoadFloat4(&leftPlane);
	m_planes[2] = DirectX::XMPlaneNormalize(m_planes[2]);


	// Calculate right plane of frustum.
	DirectX::XMFLOAT4 rightPlane;
	rightPlane.x = finalFloat4x4._14 - finalFloat4x4._11;
	rightPlane.y = finalFloat4x4._24 - finalFloat4x4._21;
	rightPlane.z = finalFloat4x4._34 - finalFloat4x4._31;
	rightPlane.w = finalFloat4x4._44 - finalFloat4x4._41;
	m_planes[3] = DirectX::XMLoadFloat4(&rightPlane);
	m_planes[3] = DirectX::XMPlaneNormalize(m_planes[3]);


	// Calculate top plane of frustum.
	DirectX::XMFLOAT4 topPlane;
	topPlane.x = finalFloat4x4._14 - finalFloat4x4._12;
	topPlane.y = finalFloat4x4._24 - finalFloat4x4._22;
	topPlane.z = finalFloat4x4._34 - finalFloat4x4._32;
	topPlane.w = finalFloat4x4._44 - finalFloat4x4._42;
	m_planes[4] = DirectX::XMLoadFloat4(&topPlane);
	m_planes[4] = DirectX::XMPlaneNormalize(m_planes[4]);


	// Calculate bottom plane of frustum.
	DirectX::XMFLOAT4 bottomPlane;
	bottomPlane.x = finalFloat4x4._14 + finalFloat4x4._12;
	bottomPlane.y = finalFloat4x4._24 + finalFloat4x4._22;
	bottomPlane.z = finalFloat4x4._34 + finalFloat4x4._32;
	bottomPlane.w = finalFloat4x4._44 + finalFloat4x4._42;
	m_planes[5] = DirectX::XMLoadFloat4(&bottomPlane);
	m_planes[5] = DirectX::XMPlaneNormalize(m_planes[5]);


}

bool Frustum::CheckPoint(float x, float y, float z)
{
	DirectX::XMVECTOR Point = DirectX::XMVectorSet(x, y, z, 1.0f);
	for (int i = 0; i < 6; ++i)
	{
		DirectX::XMFLOAT4 DotEnd;
		DirectX::XMStoreFloat4(&DotEnd, DirectX::XMPlaneDotCoord(m_planes[i], Point));
		if (DotEnd.x < 0)
		{
			return false;
		}
	}

	return true;
}

bool Frustum::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
{
	DirectX::XMVECTOR Point[8];
	DirectX::XMFLOAT4 DotEnd;
	Point[0] = DirectX::XMVectorSet(xCenter - radius, yCenter - radius, zCenter - radius, 1.0f);
	Point[1] = DirectX::XMVectorSet(xCenter + radius, yCenter - radius, zCenter - radius, 1.0f);
	Point[2] = DirectX::XMVectorSet(xCenter - radius, yCenter + radius, zCenter - radius, 1.0f);
	Point[3] = DirectX::XMVectorSet(xCenter - radius, yCenter - radius, zCenter + radius, 1.0f);
	Point[4] = DirectX::XMVectorSet(xCenter + radius, yCenter + radius, zCenter - radius, 1.0f);
	Point[5] = DirectX::XMVectorSet(xCenter + radius, yCenter - radius, zCenter + radius, 1.0f);
	Point[6] = DirectX::XMVectorSet(xCenter - radius, yCenter + radius, zCenter + radius, 1.0f);
	Point[7] = DirectX::XMVectorSet(xCenter + radius, yCenter + radius, zCenter + radius, 1.0f);

	for (int i = 0; i < 6; ++i)
	{
		XMStoreFloat4(&DotEnd, DirectX::XMPlaneDotCoord(m_planes[i], Point[0]));
		if (DotEnd.x >= 0)
		{
			continue;
		}
		XMStoreFloat4(&DotEnd, DirectX::XMPlaneDotCoord(m_planes[i], Point[1]));
		if (DotEnd.x >= 0)
		{
			continue;
		}
		XMStoreFloat4(&DotEnd, DirectX::XMPlaneDotCoord(m_planes[i], Point[2]));
		if (DotEnd.x >= 0)
		{
			continue;
		}
		XMStoreFloat4(&DotEnd, DirectX::XMPlaneDotCoord(m_planes[i], Point[3]));
		if (DotEnd.x >= 0)
		{
			continue;
		}
		XMStoreFloat4(&DotEnd, DirectX::XMPlaneDotCoord(m_planes[i], Point[4]));
		if (DotEnd.x >= 0)
		{
			continue;
		}
		XMStoreFloat4(&DotEnd, DirectX::XMPlaneDotCoord(m_planes[i], Point[5]));
		if (DotEnd.x >= 0)
		{
			continue;
		}

		XMStoreFloat4(&DotEnd, DirectX::XMPlaneDotCoord(m_planes[i], Point[6]));
		if (DotEnd.x >= 0)
		{
			continue;
		}

		XMStoreFloat4(&DotEnd, DirectX::XMPlaneDotCoord(m_planes[i], Point[7]));
		if (DotEnd.x >= 0)
		{
			continue;
		}

		return false;
	}
	return true;
}

bool Frustum::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
{
	DirectX::XMVECTOR Point = DirectX::XMVectorSet(xCenter, yCenter, zCenter, 1.0f);
	DirectX::XMFLOAT4 DotEnd;
	for (int i = 0; i < 6; ++i)
	{
		DirectX::XMStoreFloat4(&DotEnd, DirectX::XMPlaneDotCoord(m_planes[i], Point));
		if (-radius > DotEnd.x)
		{
			return false;
		}
	}
	return true;
}

bool Frustum::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
{
	DirectX::XMVECTOR Point[8];
	DirectX::XMFLOAT4 DotEnd;
	Point[0] = DirectX::XMVectorSet(xCenter - xSize, yCenter - ySize, zCenter - zSize, 1.0f);
	Point[1] = DirectX::XMVectorSet(xCenter + xSize, yCenter - ySize, zCenter - zSize, 1.0f);
	Point[2] = DirectX::XMVectorSet(xCenter - xSize, yCenter + ySize, zCenter - zSize, 1.0f);
	Point[3] = DirectX::XMVectorSet(xCenter - xSize, yCenter - ySize, zCenter + zSize, 1.0f);
	Point[4] = DirectX::XMVectorSet(xCenter + xSize, yCenter + ySize, zCenter - zSize, 1.0f);
	Point[5] = DirectX::XMVectorSet(xCenter + xSize, yCenter - ySize, zCenter + zSize, 1.0f);
	Point[6] = DirectX::XMVectorSet(xCenter - xSize, yCenter + ySize, zCenter + zSize, 1.0f);
	Point[7] = DirectX::XMVectorSet(xCenter + xSize, yCenter + ySize, zCenter + zSize, 1.0f);

	for (int i = 0; i < 6; ++i)
	{
		XMStoreFloat4(&DotEnd, DirectX::XMPlaneDotCoord(m_planes[i], Point[0]));
		if (DotEnd.x >= 0)
		{
			continue;
		}


		XMStoreFloat4(&DotEnd, DirectX::XMPlaneDotCoord(m_planes[i], Point[1]));
		if (DotEnd.x >= 0)
		{
			continue;
		}


		XMStoreFloat4(&DotEnd, DirectX::XMPlaneDotCoord(m_planes[i], Point[2]));
		if (DotEnd.x >= 0)
		{
			continue;
		}

		XMStoreFloat4(&DotEnd, DirectX::XMPlaneDotCoord(m_planes[i], Point[3]));
		if (DotEnd.x >= 0)
		{
			continue;
		}

		XMStoreFloat4(&DotEnd, DirectX::XMPlaneDotCoord(m_planes[i], Point[4]));
		if (DotEnd.x >= 0)
		{
			continue;
		}


		XMStoreFloat4(&DotEnd, DirectX::XMPlaneDotCoord(m_planes[i], Point[5]));
		if (DotEnd.x >= 0)
		{
			continue;
		}


		XMStoreFloat4(&DotEnd, DirectX::XMPlaneDotCoord(m_planes[i], Point[6]));
		if (DotEnd.x >= 0)
		{
			continue;
		}


		XMStoreFloat4(&DotEnd, DirectX::XMPlaneDotCoord(m_planes[i], Point[7]));
		if (DotEnd.x >= 0)
		{
			continue;
		}

		return false;
	}
	return true;
}
