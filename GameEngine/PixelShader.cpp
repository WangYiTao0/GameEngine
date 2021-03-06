#include "PixelShader.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"
#include "StringHelper.h"

namespace Bind
{
	PixelShader::PixelShader(Graphics& gfx, const std::string& shaderName)
		:
		m_ShaderName(shaderName)
	{
		INFOMAN(gfx);

		std::string csofolder = StrH::GetShaderRootPath();
		auto m_csoPath = csofolder + shaderName + ".cso";
		auto m_WcsoPath = StrH::ToWide(m_csoPath);

		std::string hlslfolder = "Shader\\";
		auto m_hlslPath = hlslfolder + shaderName + ".hlsl";
		auto m_WhlslPath = StrH::ToWide(m_hlslPath);

		//create vertex shader
		if (m_WcsoPath.c_str() && D3DReadFileToBlob(m_WcsoPath.c_str(), &pBlobOut) == S_OK)
		{
			
		}
		else
		{
			DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
			//allow to change global variablese in  shader
			//dwShaderFlags |=D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY;
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

	std::shared_ptr<PixelShader> PixelShader::Resolve(Graphics& gfx, const std::string& shaderName)
	{
		return Codex::Resolve<PixelShader>(gfx, shaderName);
	}
	std::string PixelShader::GenerateUID(const std::string& shaderName)
	{
		using namespace std::string_literals;
		return typeid(PixelShader).name() + "#"s + shaderName;
	}
	std::string PixelShader::GetUID() const noexcept
	{
		return GenerateUID(m_ShaderName);
	}
}
