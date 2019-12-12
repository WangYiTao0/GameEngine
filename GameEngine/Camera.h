#pragma once
#include "Graphics.h"
#include "GameObject.h"
#include "MathHelper.h"


class Camera3D : public GameObject
{
public:
	Camera3D();
	void Set3DProj(float fov, float aspec, float nearZ, float farZ);

	DirectX::XMMATRIX GetProj();
	DirectX::XMMATRIX GetViewMatrix() noexcept;

	void RenderReflection(float height);
	DirectX::XMMATRIX GetReflectionViewMatrix();
	
	inline DirectX::XMFLOAT3 GetUp()const { return m_Up; }
	inline DirectX::XMFLOAT3 GetRight()const { return m_Right; }
	inline DirectX::XMFLOAT3 GetLook()const { return m_Look; }
	inline float GetNearZ() { return m_NearZ; }
	inline float GetFarZ() { return m_FarZ; }
	inline float GetFov() { return m_Fov; }


	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void Rotate(float dx, float dy) noexcept;
	void Translate(DirectX::XMFLOAT3 translation) noexcept;

	static std::shared_ptr<Camera3D> Get();

private:
	static std::shared_ptr<Camera3D> m_pCamera;

	DirectX::XMFLOAT3 m_Up = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	DirectX::XMFLOAT3 m_Right = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 m_Look = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT4X4 m_view;

	DirectX::XMMATRIX proj;
	DirectX::XMMATRIX m_reflectionViewMatrix;

	float m_Fov = 0.0f;
	float m_Aspect = 0.0f;
	float m_NearZ = 1.0f;
	float m_FarZ = 100.0f;

	float mNearWindowHeight = 0.0f;
	float mFarWindowHeight = 0.0f;

	bool mViewDirty = true;

	static constexpr float travelSpeed = 12.0f;
	static constexpr float rotationSpeed = 0.004f;
};

class Camera2D : public GameObject
{
public:
	Camera2D();
	void SetOrtho(float width, float height, float nearZ, float farZ);
	const DirectX::XMMATRIX& GetOrthoMatrix() const;
	DirectX::XMMATRIX GetWorldMatrix();
private:
	DirectX::XMMATRIX orthoMatrix;
	
};

#define	GCamera3D (Camera3D::Get())