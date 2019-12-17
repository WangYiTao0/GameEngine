#include "VertexShader.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"
#include "StringHelper.h"
#include <typeinfo>
namespace Bind
{
	VertexShader::VertexShader(Graphics& gfx, const std::string& shaderName)
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
		if(m_WcsoPath.c_str() && D3DReadFileToBlob(m_WcsoPath.c_str(), &pBytecodeBlob) == S_OK)
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
			D3DCompileFromFile(m_WhlslPath.c_str(), 
				nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0",
				dwShaderFlags, 0, &pBytecodeBlob, &errorBlob);


			if (m_WcsoPath.c_str())
			{
				D3DWriteBlobToFile(pBytecodeBlob.Get(), m_WcsoPath.c_str(), FALSE);
			}
		}

		GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(
			pBytecodeBlob->GetBufferPointer(),
			pBytecodeBlob->GetBufferSize(), nullptr, &pVertexShader));


	}

	void VertexShader::Bind(Graphics& gfx) noexcept
	{
		//bind vertex shader
		GetContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u);

	}

	ID3DBlob* VertexShader::GetBytecode() const noexcept
	{
		return pBytecodeBlob.Get();
	}

	std::shared_ptr<VertexShader> VertexShader::Resolve(Graphics& gfx, const std::string& shaderName)
	{
		return Codex::Resolve<VertexShader>(gfx, shaderName);
	}
	std::string VertexShader::GenerateUID(const std::string& shaderName)
	{
		using namespace std::string_literals;
		return typeid(VertexShader).name() + "#"s + shaderName;
	}
	std::string VertexShader::GetUID() const noexcept
	{
		return GenerateUID(m_ShaderName);
	}
}
