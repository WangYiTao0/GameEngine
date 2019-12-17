#pragma once
#include "Drawable.h"
#include "GameObject.h"
#include <wrl/client.h>

class SkyRender :public Drawable, public GameObject
{
public:
	SkyRender(Graphics& gfx, std::vector<std::string>& filePaths, float skySphereRadius);
	SkyRender(Graphics& gfx, std::string filePaths, float skySphereRadius,bool onetex = true);
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

	ID3D11ShaderResourceView* GetTextureCube() { return m_pTextureCubeSRV.Get(); }
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTextureCubeSRV;
};