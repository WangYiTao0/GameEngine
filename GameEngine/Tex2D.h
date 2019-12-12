#pragma once
#include "Drawable.h"
#include "GameObject.h"
#include "IndexedTriangleList.h"
#include "RTT.h"

class Tex2D :public Drawable ,public GameObject
{
public:
	Tex2D(Graphics& gfx,float screenWidth,float screenHeight, float texWidth, float texHeight, std::string filePath);
	Tex2D(Graphics& gfx, float screenWidth, float screenHeight,
		float width, float height, std::string VS_Name, std::string PS_Name,ID3D11ShaderResourceView* pSv);


	void Update(Graphics& gfx);
	
	DirectX::XMMATRIX GetTransformXM() const noexcept;
private:
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 color;
	};

	int mScrrenWidth, mScrrenHeight;
	int mTexWidth, mTexHeight;

	int mPreviousPosX, mPreviousPosY;


	static IndexedTriangleList tex2D;
};