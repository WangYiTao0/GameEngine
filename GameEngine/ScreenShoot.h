//#pragma once
//#include <DirectXTK/ScreenGrab.h>
//#include "Graphics.h"
//#include "Bindable.h"
//#include "StringHelper.h"
//
//class ScreenShoot : public Bind::Bindable
//{
//public:
//	ScreenShoot() = default;
//
//	void Save(Graphics& gfx, std::string name,ID3D11Resource* pSRV)
//	{
//		using namespace Bind;
//		DirectX::SaveWICTextureToFile
//		(
//			GetContext(gfx),
//			pSRV,
//			StrH::ToWide(name).c_str(),
//
//
//			);
//	}
//
//
//};