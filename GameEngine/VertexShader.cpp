#include "VertexShader.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"
#include "StringHelper.h"
#include <typeinfo>
namespace Bind
{
	VertexShader::VertexShader(Graphics& gfx, const std::string& csoPath, const std::string& hlslPath)
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

	std::shared_ptr<VertexShader> VertexShader::Resolve(Graphics& gfx, const std::string& csoPath, const std::string& hlslPath)
	{
		return Codex::Resolve<VertexShader>(gfx, csoPath, hlslPath);
	}
	std::string VertexShader::GenerateUID(const std::string& csoPath, const std::string& hlslPath)
	{
		using namespace std::string_literals;
		return typeid(VertexShader).name() + "#"s + csoPath + hlslPath;
	}
	std::string VertexShader::GetUID() const noexcept
	{
		return GenerateUID(csoPath,hlslPath);
	}
}
