#pragma once
#ifndef LIGHTCOMMON_H_
#define LIGHTCOMMON_H_
#include <DirectXMath.h>
#include "MathHelper.h"

struct PointLightCBuf
{
	DirectX::XMFLOAT3 position;
	float Intensity;
	DirectX::XMFLOAT3 ambient;
	float attConst;
	DirectX::XMFLOAT3 diffuse;
	float attLin;
	DirectX::XMFLOAT3 cameraPos;
	float attQuad;
};

struct DirectionLight
{
	DirectX::XMFLOAT3 direction;
	float intensity;
};

struct SpotLight
{
	DirectX::XMFLOAT3 position;
	float intensity;
	DirectX::XMFLOAT3 direction;
	float spotAngle;
	DirectX::XMFLOAT3 spotPower;
};

struct Material
{
	DirectX::XMFLOAT3 ambient;
	float attConst;
	DirectX::XMFLOAT3 diffuse;
	float attLin;
	DirectX::XMFLOAT3 specular;
};

struct Light
{
	DirectionLight directionLight;
	PointLightCBuf pointLight;
	SpotLight spotLight;
};

#define MaxLights 16

struct MaterialConstants
{
	DirectX::XMFLOAT4 DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT3 FresnelR0 = { 0.01f, 0.01f, 0.01f };
	float Roughness = 0.25f;


};

// Simple struct to represent a material for our demos.  A production 3D engine
// would likely create a class hierarchy of Materials.
//struct Material
//{
//	// Unique material name for lookup.
//	std::string Name;
//
//	// Index into constant buffer corresponding to this material.
//	int MatCBIndex = -1;
//
//	// Material constant buffer data used for shading.
//	DirectX::XMFLOAT4 DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
//	DirectX::XMFLOAT3 FresnelR0 = { 0.01f, 0.01f, 0.01f };
//	float Roughness = .25f;
//	DirectX::XMFLOAT4X4 MatTransform = MathHelper::Identity4x4();
//};


//BRDF 
struct BRDF_Surface
{
	DirectX::XMFLOAT3 N;
	float roughness; // 0~1;
	DirectX::XMFLOAT3 diffuseColor;
	float metalness;// 0~1;
	DirectX::XMFLOAT3 specularColor;
	DirectX::XMFLOAT3 emissiveColor;// self lightColor
	float emissiveIntensity;
};

#endif // !LIGHTCOMMON_H_