#pragma once
#include<cmath>
#include<random>

namespace MH
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

	static DirectX::XMFLOAT3 GetCameraPosFromViewInvMatrix(DirectX::XMMATRIX viewInv)
	{
		/*Inverse the view matrix by calling XMMatrixInverse or
		D3DXMatrixInverse so you get the "CameraWorld" matrix.
		Then its (_41,_42,_43) elements would be the position vector.

			Inverse the view matrix (such as last time) but
			instead of reading forth roe, use XMMatrixDecompose
			or D3DXMatrixDecompose to get camera position and orientation.*/

		DirectX::XMVECTOR scale;
		DirectX::XMVECTOR rot;
		DirectX::XMVECTOR trans;
		DirectX::XMFLOAT3 trans3f;

		DirectX::XMMatrixDecompose(&scale, &rot, &trans, viewInv);
		DirectX::XMStoreFloat3(&trans3f, trans);
		return	trans3f;
	}

	class Vec3 : public DirectX::XMFLOAT3
	{
	public:
		Vec3() = default;
		Vec3(const float x, const float y, const float z)
			:
			DirectX::XMFLOAT3(x, y, z)
		{}
		Vec3(const DirectX::XMFLOAT3& src)
		{
			x = src.x;
			y = src.y;
			z = src.z;
		}
		Vec3(const DirectX::XMVECTOR v)
		{
			DirectX::XMStoreFloat3(this, v);
		}

		Vec3& operator=(const DirectX::XMFLOAT3& src)
		{
			x = src.x;
			y = src.y;
			z = src.z;

			return *this;
		}
		Vec3 operator*(float scalar) const
		{
			Vec3 res;
			res.x = x * scalar;
			res.y = y * scalar;
			res.z = z * scalar;

			return res;
		}
		Vec3 operator-() const
		{
			Vec3 res;
			res.x = -x;
			res.y = -y;
			res.z = -z;

			return res;
		}
		Vec3 operator+(const Vec3& rhs) const
		{
			Vec3 res;
			res.x = x + rhs.x;
			res.y = y + rhs.y;
			res.z = z + rhs.z;

			return res;
		}
		Vec3 operator-(const Vec3& rhs) const
		{
			Vec3 res;
			res.x = x - rhs.x;
			res.y = y - rhs.y;
			res.z = z - rhs.z;

			return res;
		}
		Vec3& operator+=(const Vec3& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;

			return *this;
		}
		Vec3& operator-=(const Vec3& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;

			return *this;
		}

		float LengthSq() const
		{
			return x * x + y * y + z * z;
		}

		float Length() const
		{
			return sqrt(LengthSq());
		}

		Vec3& Normalize()
		{
			float length = Length();
			x /= length;
			y /= length;
			z /= length;

			return *this;
		}

		Vec3 Normalized() const
		{
			float length = Length();

			return { x / length, y / length, z / length };
		}

		operator DirectX::XMVECTOR() const
		{
			return DirectX::XMLoadFloat3(this);
		}
	};

	class Vec4 : public DirectX::XMFLOAT4
	{
	public:
		Vec4() = default;
		Vec4(const float x, const float y, const float z, const float w)
			:
			DirectX::XMFLOAT4(x, y, z, w)
		{}
		Vec4(const Vec3& v3, const float vw)
		{
			x = v3.x;
			y = v3.y;
			z = v3.z;
			w = vw;
		}

		Vec4(const DirectX::XMFLOAT4& src)
		{
			x = src.x;
			y = src.y;
			z = src.z;
			w = src.w;
		}
		Vec4(const DirectX::XMVECTOR v)
		{
			DirectX::XMStoreFloat4(this, v);
		}

		Vec4& operator=(const DirectX::XMFLOAT4& src)
		{
			x = src.x;
			y = src.y;
			z = src.z;
			w = src.w;

			return *this;
		}
		Vec4& operator=(const DirectX::XMFLOAT3& src)
		{
			x = src.x;
			y = src.y;
			z = src.z;
			w = 1.0f;

			return *this;
		}
		Vec4 operator*(float scalar) const
		{
			Vec4 res;
			res.x = x * scalar;
			res.y = y * scalar;
			res.z = z * scalar;
			res.w = w * scalar;

			return res;
		}
		Vec4 operator/(float scalar) const
		{
			Vec4 res;
			res.x = x / scalar;
			res.y = y / scalar;
			res.z = z / scalar;
			res.w = w / scalar;

			return res;
		}
		Vec4 operator-() const
		{
			Vec4 res;
			res.x = -x;
			res.y = -y;
			res.z = -z;
			res.w = -w;

			return res;
		}

		Vec4& operator*=(float scalar)
		{
			x = x * scalar;
			y = y * scalar;
			z = z * scalar;
			w = w * scalar;

			return *this;
		}
		Vec4& operator/=(float scalar)
		{
			x = x / scalar;
			y = y / scalar;
			z = z / scalar;
			w = w / scalar;

			return *this;
		}

		operator DirectX::XMVECTOR() const
		{
			return DirectX::XMLoadFloat4(this);
		}

		static float Dot(const Vec4& a, const Vec4& b)
		{
			return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
		}
	};
}
