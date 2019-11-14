#pragma once
#include <wrl/client.h>
#include <d3d11.h>

class RenderStates
{
public:

	static bool IsInit();

	static void InitAll(ID3D11Device* device);


public:
	static  Microsoft::WRL::ComPtr<ID3D11RasterizerState> RSWireframe;	// RasterizerState�FWireframe
	static  Microsoft::WRL::ComPtr<ID3D11RasterizerState> RSNoCull;		// RasterizerState�FNoBackCull

	static Microsoft::WRL::ComPtr<ID3D11SamplerState> SSLinearWrap;		// SamplerState�FLinear filtering
	static Microsoft::WRL::ComPtr<ID3D11SamplerState> SSAnistropicWrap;	// SamplerState�FAnistropicWrap

	static Microsoft::WRL::ComPtr<ID3D11BlendState> BSNoColorWrite;		// BlendState�FNoColorWrite
	static Microsoft::WRL::ComPtr<ID3D11BlendState> BSTransparent;		// BlendState�FTransparent
	static Microsoft::WRL::ComPtr<ID3D11BlendState> BSAlphaToCoverage;	// BlendState�FAlpha-To-Coverage
};


