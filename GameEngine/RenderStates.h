#pragma once
#include <wrl/client.h>
#include <d3d11.h>

class RenderStates
{
public:

	static bool IsInit();

	static void InitAll(ID3D11Device* device);


public:
	static  Microsoft::WRL::ComPtr<ID3D11RasterizerState> RSWireframe;	// RasterizerState：Wireframe
	static  Microsoft::WRL::ComPtr<ID3D11RasterizerState> RSNoCull;		// RasterizerState：NoBackCull

	static Microsoft::WRL::ComPtr<ID3D11SamplerState> SSLinearWrap;		// SamplerState：Linear filtering
	static Microsoft::WRL::ComPtr<ID3D11SamplerState> SSAnistropicWrap;	// SamplerState：AnistropicWrap

	static Microsoft::WRL::ComPtr<ID3D11BlendState> BSNoColorWrite;		// BlendState：NoColorWrite
	static Microsoft::WRL::ComPtr<ID3D11BlendState> BSTransparent;		// BlendState：Transparent
	static Microsoft::WRL::ComPtr<ID3D11BlendState> BSAlphaToCoverage;	// BlendState：Alpha-To-Coverage
};


