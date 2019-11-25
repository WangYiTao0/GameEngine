#include "VertexShader.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"
#include "StringHelper.h"
#include <typeinfo>
namespace Bind
{
	VertexShader::VertexShader(Graphics& gfx, const std::string& path)
		:
		path(path)
	{
		INFOMAN(gfx);

		std::string shaderfolder = StringHelper::GetShaderRootPath();

		auto m_Path = shaderfolder + path;

		//create vertex shader
		GFX_THROW_INFO(D3DReadFileToBlob(std::wstring{ m_Path.begin(), m_Path.end()}.c_str(), &pBytecodeBlob));
		


		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG

		dwShaderFlags |= D3DCOMPILE_DEBUG;

		dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		
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

	std::shared_ptr<VertexShader> VertexShader::Resolve(Graphics& gfx, const std::string& path)
	{
		return Codex::Resolve<VertexShader>(gfx, path);
	}
	std::string VertexShader::GenerateUID(const std::string& path)
	{
		using namespace std::string_literals;
		return typeid(VertexShader).name() + "#"s + path;
	}
	std::string VertexShader::GetUID() const noexcept
	{
		return GenerateUID(path);
	}
}
