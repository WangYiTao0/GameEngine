#include "PixelShader.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"
#include "StringHelper.h"

namespace Bind
{
	PixelShader::PixelShader(Graphics& gfx, const std::string& csoPath, const std::string& hlslPath)
		:
		csoPath(csoPath),
		hlslPath(hlslPath)
	{
		INFOMAN(gfx);

		std::string csofolder = StringHelper::GetShaderRootPath();
		auto m_csoPath = csofolder + csoPath;
		auto m_WcsoPath = std::wstring{ m_csoPath.begin(), m_csoPath.end() };

		std::string hlslfolder = "Shader\\";
		auto m_hlslPath = hlslfolder + hlslPath;
		auto m_WhlslPath = std::wstring{ m_hlslPath.begin(),m_hlslPath.end() };


		//create vertex shader
		if (m_WcsoPath.c_str() && D3DReadFileToBlob(m_WcsoPath.c_str(), &pBlobOut) == S_OK)
		{
			
		}
		else
		{
			DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG

			dwShaderFlags |= D3DCOMPILE_DEBUG;

			dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;

#endif
			Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
			GFX_THROW_INFO(D3DCompileFromFile(m_WhlslPath.c_str(),
				nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0",
				dwShaderFlags, 0, pBlobOut.GetAddressOf(), errorBlob.GetAddressOf()));


			if (m_WcsoPath.c_str())
			{
				D3DWriteBlobToFile(pBlobOut.Get(), m_WcsoPath.c_str(), FALSE);
			}
		}
		//create pixel shader
		GFX_THROW_INFO(GetDevice(gfx)->CreatePixelShader(pBlobOut->GetBufferPointer(),
			pBlobOut->GetBufferSize(), nullptr, &pPixelShader));

	}

	void PixelShader::Bind(Graphics& gfx) noexcept
	{
		//bind pixel shader
		GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	}

	std::shared_ptr<PixelShader> PixelShader::Resolve(Graphics& gfx, const std::string& csoPath, const std::string& hlslPath)
	{
		return Codex::Resolve<PixelShader>(gfx, csoPath, hlslPath);
	}
	std::string PixelShader::GenerateUID(const std::string& csoPath, const std::string& hlslPath)
	{
		using namespace std::string_literals;
		return typeid(PixelShader).name() + "#"s + csoPath + hlslPath;
	}
	std::string PixelShader::GetUID() const noexcept
	{
		return GenerateUID(csoPath, hlslPath);
	}
}
