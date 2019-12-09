#pragma once
#include<cmath>
#include<random>

namespace MathHelper
{
	constexpr float PI = 3.14159265f;
	constexpr double PI_D = 3.1415926535897932;
	constexpr float oneRad = 0.0174532925f;

	template<typename T>
	constexpr auto sq(const T& x)
	{
		return x * x;
	}

	//we don't need large rad  
	// limit rad in -1 ~ +1
	template<typename T>
	//set angle between -180 ande 180
	T wrap_angle(T theta)
	{
		const T modded = fmod(theta, (T)2.0 * (T)PI_D);
		return (modded > (T)PI_D) ?
			(modded - (T)2.0 * (T)PI_D) :
			modded;
	}
	//lerp
	template<typename T>
	constexpr T interpolate(const T& src, const T& dst, float alpha)
	{
		return src + (dst - src) * alpha;
	}

	//degree to rad
	template<typename T>
	constexpr T to_rad(T deg)
	{
		return deg * PI / (T)180.0;
	}


	template<typename T>
	T RandT(T a, T b)
	{
		std::mt19937 rng(std::random_device{}());
		std::uniform_real_distribution<T> rdist(a, b);

		return rdist(rng);
	}

	template<typename T>
	T RandI(T a, T b)
	{
		std::mt19937 rng(std::random_device{}());
		std::uniform_int_distribution<T> rdist(a, b);

		return rdist(rng);
	}

	static DirectX::XMMATRIX Inversse(DirectX::CXMMATRIX M)
	{
		DirectX::XMMATRIX A = M;
		A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(A);
		return DirectX::XMMatrixInverse(&det, A);
	}

	static DirectX::XMMATRIX InverseTranspose(DirectX::CXMMATRIX M)
	{
		// Inverse-transpose is just applied to normals.  So zero out 
		// translation row so that it doesn't get into our inverse-transpose
		// calculation--we don't want the inverse-transpose of the translation.
		DirectX::XMMATRIX A = M;
		A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(A);
		return DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&det, A));
	}

	static DirectX::XMFLOAT4X4 Identity4x4()
	{
		static DirectX::XMFLOAT4X4 I(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		return I;
	}

	static DirectX::XMFLOAT3 GetCameraPosFromViewMatrix(DirectX::XMMATRIX viewInv)
	{
		/*Inverse the view matrix by calling XMMatrixInverse or
		D3DXMatrixInverse so you get the "CameraWorld" matrix.
		Then its (_41,_42,_43) elements would be the position vector.

			Inverse the view matrix (such as last time) but
			instead of reading forth roe, use XMMatrixDecompose 
			or D3DXMatrixDecompose to get camera position and orientation.*/
		DirectX::XMFLOAT4X4 fViewInv;
		DirectX::XMStoreFloat4x4(&fViewInv, viewInv);
		return{ fViewInv._41, fViewInv._42, fViewInv._43 };
	}
}
