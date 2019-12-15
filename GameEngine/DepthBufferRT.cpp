#include "DepthBufferRT.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"
#include "StringHelper.h"
#include "Surface.h"

namespace Bind
{
	Bind::DepthBufferRT::DepthBufferRT(Graphics& gfx, int textureWidth, int textureHeight)
	
	{
		INFOMAN(gfx);

		//第一,填充深度视图的2D纹理形容结构体,并创建2D渲染纹理
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
		depthBufferDesc.Width = textureWidth;
		depthBufferDesc.Height = textureHeight;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS; //24位是为了深度缓存，8位是为了模板缓存
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;  //注意深度缓存(纹理)的绑定标志
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;
		GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(&depthBufferDesc, NULL, m_pDepthBackBufferTexture2D.GetAddressOf()));

		//第二,填充深度缓存视图形容结构体,并创建深度缓存视图
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		GFX_THROW_INFO(GetDevice(gfx)->CreateDepthStencilView(m_pDepthBackBufferTexture2D.Get(), &depthStencilViewDesc, m_pDephtStencilView.GetAddressOf()));


		//第三,填充着色器资源视图形容体,并进行创建着色器资源视图,注意这是用深度缓存(纹理)来创建的，而不是渲染目标缓存(纹理)创建的
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS; //此时因为是仅仅进行深度写，而不是颜色写，所以此时Shader资源格式跟深度缓存是一样的
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = depthBufferDesc.MipLevels;
		GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(m_pDepthBackBufferTexture2D.Get(), &shaderResourceViewDesc, m_SRV.GetAddressOf()));

		//第四，设置视口的属性
		m_ViewPort.Width = (float)textureWidth;
		m_ViewPort.Height = (float)textureHeight;
		m_ViewPort.MinDepth = 0.0f;
		m_ViewPort.MaxDepth = 1.0f;
		m_ViewPort.TopLeftX = 0.0f;
		m_ViewPort.TopLeftY = 0.0f;
	}
	void DepthBufferRT::Bind(Graphics& gfx) noexcept
	{
		SetRenderTarget(gfx);
	}
	void DepthBufferRT::SetRenderTarget(Graphics& gfx) noexcept
	{
		//设置渲染目标视图为NULL 可以大大提高我们3D程序的性能
		ID3D11RenderTargetView* renderTarget[1] = { nullptr };
		//绑定渲染目标视图和深度模板视图到输出渲染管线
		GetContext(gfx)->OMSetRenderTargets(1, renderTarget, m_pDephtStencilView.Get());

		//设置视口
		GetContext(gfx)->RSSetViewports(1, &m_ViewPort);

		ClearDepth(gfx);

	}
	//不用清除背后缓存,因为不需要进颜色写(ColorWrite),仅仅进行深度写
	void DepthBufferRT::ClearDepth(Graphics& gfx)
	{
		GetContext(gfx)->ClearDepthStencilView(m_pDephtStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	ID3D11ShaderResourceView* DepthBufferRT::GetShaderResourceView()
	{
		return m_SRV.Get();
	}
}