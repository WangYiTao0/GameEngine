#pragma once
#include "WindowHelper.h"
#include "BaseException.h"
#include <d3d11.h>
#include <wrl.h>
#include <vector>
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
	class Exception : public BaseException
	{
		using BaseException::BaseException;
	};
	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
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
	class DeviceRemovedException : public HrException
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

	
	ID3D11DepthStencilView* GetDepthStencilView();

	void EndFrame();
	void BeginFrame(float red, float green, float blue) noexcept;

	void DrawIndexedInstance(UINT indexCount, UINT instanceCount)noexcept(!IS_DEBUG);
	void DrawIndexed(UINT indexCount) noxnd;
	//camera 3d
	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;
	void SetCameraViewMatirx(DirectX::FXMMATRIX cam) noexcept;
	DirectX::XMMATRIX GetCameraViewMatrix() const noexcept;
	//camera 2d
	void SetOrtho(DirectX::FXMMATRIX proj) noexcept;
	DirectX::XMMATRIX GetOrtho() const noexcept;
	void SetCamera2DWorldMatirx(DirectX::FXMMATRIX cam) noexcept;
	DirectX::XMMATRIX GetCamera2DWorldMatrix() const noexcept;

	float* GetClearColor();

	void SetBackBufferRenderTarget();

	void EnableImgui() noexcept;
	void DisableImgui() noexcept;
	bool IsImguiEnabled() const noexcept;
private:
	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX Ortho;
	DirectX::XMMATRIX camera2D;
	DirectX::XMMATRIX camera3D;

	float color[4] = {1.0f,1.0f,1.0f,1.0f};

	bool imguiEnabled = true;
#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;

	UINT      m_4xMsaaQuality;   // MSAA


};