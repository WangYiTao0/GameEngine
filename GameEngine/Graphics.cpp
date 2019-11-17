#include "Graphics.h"
#include "dxerr.h"
#include <sstream>
#include <d3dcompiler.h>
#include <cmath>
#include <DirectXMath.h>
#include "GraphicsThrowMacros.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

Graphics::Graphics(HWND hWnd, int width, int height)
	:
	m_ClientWidth(width),
	m_ClientHeight(height),
	m_hMainWnd(hWnd),
	m_Minimized(false),
	m_Maximized(false),
	m_Resizing(false),
	m_Enable4xMsaa(true),
	m_4xMsaaQuality(0),
	m_pDevice(nullptr),
	m_pContext(nullptr),
	m_pSwapChain(nullptr),
	m_pDepthStencilBuffer(nullptr),
	m_pRenderTargetView(nullptr),
	m_pDepthStencilView(nullptr)
{
	InitializeDirectX();

	// init imgui d3d impl
	ImGui_ImplDX11_Init(m_pDevice.Get(), m_pContext.Get());
}

Graphics::~Graphics()
{
	if (m_pContext)
		m_pContext->ClearState();

	ImGui_ImplDX11_Shutdown();
}

void Graphics::OnResize()
{
	HRESULT hr;

	assert(m_pContext);
	assert(m_pDevice);
	assert(m_pSwapChain);

	//if (m_pDevice1 != nullptr)
	//{
	//	assert(m_pContext1);
	//	assert(m_pDevice1);
	//	assert(m_pSwapChain1);
	//}

	// Release all outstanding references to the swap chain's buffers.
	// Reset the Rending pipeline  source view
	m_pRenderTargetView.Reset();
	m_pDepthStencilView.Reset();
	m_pDepthStencilBuffer.Reset();

	// Reset the swapchain  re create  render target view
	HR(m_pSwapChain->ResizeBuffers(1,
		m_ClientWidth, m_ClientHeight,
		DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, 0));	// DXGI_FORMAT_B8G8R8A8_UNORM

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	HR(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf())));
	HR(m_pDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_pRenderTargetView.GetAddressOf()));

	backBuffer.Reset();

	// create depth stensil texture
	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = UINT(m_ClientWidth);
	depthStencilDesc.Height = UINT(m_ClientHeight);
	depthStencilDesc.MipLevels = 1u;
	depthStencilDesc.ArraySize = 1u;
	depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;//D for depth
	//depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// Enable 4X MSAA?
	if (m_Enable4xMsaa)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	GFX_THROW_INFO(m_pDevice->CreateTexture2D(&depthStencilDesc,
		nullptr, &m_pDepthStencilBuffer));
	GFX_THROW_INFO(m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer.Get(),
		nullptr, m_pDepthStencilView.GetAddressOf()));




	////create depth stensil state
	//D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	//dsDesc.DepthEnable = TRUE;
	//dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	//wrl::ComPtr<ID3D11DepthStencilState> pDSState;
	//GFX_THROW_INFO(m_pDevice->CreateDepthStencilState(&dsDesc, &pDSState));

	////bind depth state
	//m_pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	//bind depth  stensil view to OM output merger
	m_pContext->OMSetRenderTargets(1u,
		m_pRenderTargetView.GetAddressOf(),
		m_pDepthStencilView.Get());

	// configure viewport
	m_ScreenViewPort.Width = FLOAT(m_ClientWidth);
	m_ScreenViewPort.Height = FLOAT(m_ClientHeight);
	m_ScreenViewPort.MinDepth = 0;
	m_ScreenViewPort.MaxDepth = 1;
	m_ScreenViewPort.TopLeftX = 0;
	m_ScreenViewPort.TopLeftY = 0;
	//Rasterizer Stage
	m_pContext->RSSetViewports(1u, &m_ScreenViewPort);

}

void Graphics::BeginFrame(float red, float green, float blue) noexcept
{


	// imgui begin frame
	if (m_imguiEnabled)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
	const float color[] = { red,green,blue,1.0f };
	m_pContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
	m_pContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);


}

void Graphics::EndFrame()
{
	// imgui frame end
	if (m_imguiEnabled)
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	HRESULT hr;
#ifndef NDEBUG
	infoManager.Set();
#endif
	if (FAILED(hr = m_pSwapChain->Present(1u, 0u)))
	{
		// physically pull the graphics card out of the machine when it was running
		//driver crash  overclocking GPU
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw GFX_DEVICE_REMOVED_EXCEPT(m_pDevice->GetDeviceRemovedReason());
		}
		else
		{
			throw GFX_EXCEPT(hr);
		}
	}
}

void Graphics::DrawIndexed(UINT count) noxnd
{
	GFX_THROW_INFO_ONLY(m_pContext->DrawIndexed(count, 0u, 0u));
}
void Graphics::DrawIndexedInstanced(UINT count) noxnd
{
//m_pContext->DrawIndexedInstanced()
}

void Graphics::SetProjection(DirectX::FXMMATRIX proj) noexcept
{
	m_Projection = proj;
}

DirectX::XMMATRIX Graphics::GetProjection() const noexcept
{
	return m_Projection;
}

void Graphics::SetCameraViewMatirx(DirectX::FXMMATRIX cam) noexcept
{
	m_CameraViewMatrix = cam;
}

DirectX::XMMATRIX Graphics::GetCameraViewMatrix() const noexcept
{
	return m_CameraViewMatrix;
}

void Graphics::EnableImgui() noexcept
{
	m_imguiEnabled = true;
}

void Graphics::DisableImgui() noexcept
{
	m_imguiEnabled = false;
}

bool Graphics::IsImguiEnabled() const noexcept
{
	return m_imguiEnabled;
}

bool Graphics::InitializeDirectX()
{		
// for checking results of d3d functions
	HRESULT hr = S_OK;
	// Create the device and device context.
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
	//UINT createDeviceFlags = 0u;
#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif	

	//	UINT swapCreateFlags = 0u;
	//#ifndef NDEBUG
	//	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
	//#endif

	// D3D_DRIVER_TYPE
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	// D3D_FEATURE_LEVEL
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	D3D_FEATURE_LEVEL featureLevel;
	D3D_DRIVER_TYPE d3dDriverType;
	for (UINT driverTypeIndex = 0;
		driverTypeIndex < numDriverTypes;
		driverTypeIndex++)
	{
		d3dDriverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(
			nullptr,		//DXGIAdapter  default adapter
			d3dDriverType,  //DriverType
			nullptr,		// no software device
			createDeviceFlags,
			featureLevels,
			numFeatureLevels,
			D3D11_SDK_VERSION,
			m_pDevice.GetAddressOf(),
			&featureLevel,
			m_pContext.GetAddressOf());
		GFX_THROW_INFO(hr);

		if (hr == E_INVALIDARG)
		{
			// Direct3D 11.0
			hr = D3D11CreateDevice(nullptr,
				d3dDriverType,
				nullptr,
				createDeviceFlags,
				&featureLevels[1],
				numFeatureLevels - 1,
				D3D11_SDK_VERSION,
				m_pDevice.GetAddressOf(),
				&featureLevel,
				m_pContext.GetAddressOf());
			GFX_THROW_INFO(hr);
		}

		if (SUCCEEDED(hr))
			break;
	}

	if (FAILED(hr))
	{
		MessageBox(0, "D3D11CreateDevice Failed.", 0, 0);
		return false;
	}

	// check featurelevel11.0 or 11.1
	if (featureLevel != D3D_FEATURE_LEVEL_11_0 &&
		featureLevel != D3D_FEATURE_LEVEL_11_1)
	{
		MessageBox(0, "Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}

	// check MSAA level
	m_pDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality);
	assert(m_4xMsaaQuality > 0);

	wrl::ComPtr<IDXGIDevice> dxgiDevice = nullptr;
	wrl::ComPtr<IDXGIAdapter> dxgiAdapter = nullptr;
	wrl::ComPtr<IDXGIFactory1> dxgiFactory1 = nullptr;	// D3D11.0(contain DXGI1.1)
	//wrl::ComPtr<IDXGIFactory2> dxgiFactory2 = nullptr;	// D3D11.1(contain DXGI1.2)
	// "IDXGIFactory::CreateSwapChain: This function is being called with a device from a different IDXGIFactory."
	GFX_THROW_INFO(m_pDevice.As(&dxgiDevice));
	GFX_THROW_INFO(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));
	GFX_THROW_INFO(dxgiAdapter->GetParent(__uuidof(IDXGIFactory1),
		reinterpret_cast<void**>(dxgiFactory1.GetAddressOf())));


		DXGI_SWAP_CHAIN_DESC sd = {};
		// DXGI_MODE_DESC  Buffer Desc
		sd.BufferDesc.Width = m_ClientWidth;
		sd.BufferDesc.Height = m_ClientHeight;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		//DXGI_SAMPLE_DESC SampleDesc
		if (m_Enable4xMsaa)
		{
			sd.SampleDesc.Count = 4;
			sd.SampleDesc.Quality = m_4xMsaaQuality - 1;
		}
		else
		{
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
		}

		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = m_hMainWnd;
		sd.Windowed = true;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;
		//create swapchain
		//& = pSwap.ReleaseAndGetAddressOf();
		GFX_THROW_INFO(dxgiFactory1->CreateSwapChain(
			m_pDevice.Get(),
			&sd, m_pSwapChain.GetAddressOf()));
	


	//// gain access to texture subresource in swap chain (back buffer)
	//wrl::ComPtr<ID3D11Resource> pBackBuffer;
	//GFX_THROW_INFO(m_pSwapChain->GetBuffer(0,
	//	__uuidof(ID3D11Resource),
	//	&pBackBuffer));
	//GFX_THROW_INFO(m_pDevice->CreateRenderTargetView(
	//	pBackBuffer.Get(),
	//	nullptr,
	//	&m_pRenderTargetView));

	// forbid alt enter
	dxgiFactory1->MakeWindowAssociation(m_hMainWnd, DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES);

	OnResize();

	return true;
}





// Graphics exception stuff
Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
	:
	Exception(line,file),
	hr(hr)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
	// remove final newline if exists
	if (!info.empty())
	{
		info.pop_back();
	}
}

const char* Graphics::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;
	if (!info.empty())
	{
		oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}
	oss<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::HrException::GetType() const noexcept
{
	return "Chili Graphics Exception";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Graphics::HrException::GetErrorString() const noexcept
{
	return DXGetErrorString(hr);
}

std::string Graphics::HrException::GetErrorDescription() const noexcept
{
	char buf[512];
	DXGetErrorDescription(hr, buf, sizeof(buf));
	return buf;
}

std::string Graphics::HrException::GetErrorInfo() const noexcept
{
	return info;
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "Chili Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}

Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
	:
	Exception(line, file)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
	// remove final newline if exists
	if (!info.empty())
	{
		info.pop_back();
	}
}

const char* Graphics::InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::InfoException::GetType() const noexcept
{
	return "Chili Graphics Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const noexcept
{
	return info;
}
