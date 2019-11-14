#pragma once
#include "WindowHelper.h"
#include "BaseException.h"
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include <memory>
#include "DxgiInfoManager.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#include <random>
#include "ConditionalNoexcept.h"

namespace Bind
{
	class Bindable;
}


class Graphics
{
	friend Bind::Bindable;
public:
	class Exception :public BaseException
	{
		using BaseException::BaseException;
	};
	class HrException :public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {})noexcept;
		const char* what()const noexcept override;
		const char* GetType()const noexcept override;
		HRESULT GetErrorCode()	const noexcept;
		std::string GetErrorString()const noexcept;
		std::string GetErrorDescription()const noexcept;
		std::string GetErrorInfo() const noexcept;
	private:
		HRESULT hr;
		std::string info;
	};
	class InfoException : public Exception
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string info;
	};
	class DeviceRemovedException :public HrException
	{
		using HrException::HrException;
	public:
		const char* GetType() const noexcept override;
	private:
		std::string reason;
	};
public:
	Graphics(HWND hWnd, int width, int height);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
	void OnResize();
	void EndFrame();
	void BeginFrame(float red, float green, float blue) noexcept;
	void DrawIndexed(UINT count) noxnd;
	void DrawIndexedInstanced(UINT count) noxnd;
	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;
	void SetCameraViewMatirx(DirectX::FXMMATRIX cam) noexcept;
	DirectX::XMMATRIX GetCameraViewMatrix() const noexcept;// view matrix
	void EnableImgui() noexcept;
	void DisableImgui() noexcept;
	bool IsImguiEnabled() const noexcept;
private:
	bool InitializeDirectX();
private:
	bool m_imguiEnabled = true;
	DirectX::XMMATRIX m_Projection;
	DirectX::XMMATRIX m_CameraViewMatrix;
#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif
	HWND m_hMainWnd;
	int m_ClientWidth;
	int m_ClientHeight;
	bool m_Minimized;       //  minimized
	bool m_Maximized;       //  maximized
	bool m_Resizing;        //  Resizing
	bool m_Enable4xMsaa;	 //  Enable 4x Mutiple sampler
	UINT m_4xMsaaQuality; //MSAA support level
	// Direct3D 11
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;//using for allocating someting
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;//configuring a pipeline in executing or issuing rendering command
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
	//// Direct3D 11.1
	//Microsoft::WRL::ComPtr<ID3D11Device1> m_pDevice1;						// D3D11.1 device
	//Microsoft::WRL::ComPtr<ID3D11DeviceContext1> m_pContext1;		// D3D11.1 context
	//Microsoft::WRL::ComPtr<IDXGISwapChain1> m_pSwapChain1;						// D3D11.1 swapchain
	//common resource view
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;	
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer;		
	//
	D3D11_VIEWPORT m_ScreenViewPort;                          // view port
	//BlendState
	//Microsoft::WRL::ComPtr<ID3D11BlendState> m_pBlendState;
	//Microsoft::WRL::ComPtr<ID3D11Buffer>


	bool m_VsyncEnable;
	int m_VideoCardMemory;// graphic card 
	char m_VideoCardDescription[128];//graphic card name

};