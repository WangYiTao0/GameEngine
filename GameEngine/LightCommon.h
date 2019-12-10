#pragma once
#ifndef LIGHTCOMMON_H_
#define LIGHTCOMMON_H_
#include <DirectXMath.h>
#include "MathHelper.h"

#define MaxLights 16


struct LightCommon
{
	DirectX::XMFLOAT3 position;  //spot point
	float attQuad;	 //spot point
	DirectX::XMFLOAT3 diffColor; //  direct spot point 
	float attLin;	 //spot point
	DirectX::XMFLOAT3 direction; //direct spot
	float attConst;	 //spot point
	DirectX::XMFLOAT3 ambient;//  direct spot point 
	float spotPower; //spot
	DirectX::XMFLOAT3 specular; //direct spot point 
	float cutOff;//spot

	float outerCutOff;//spot
	float lightPadding[3];
};



struct Material
{
	DirectX::XMFLOAT3 diff = { 1.0f, 1.0f, 1.0f};
	float shinness = 30.0f;
	DirectX::XMFLOAT3 spec = { 0.01f, 0.01f, 0.01f };
	float padding;
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