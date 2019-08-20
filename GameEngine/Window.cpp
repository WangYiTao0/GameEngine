#include "Window.h"
#include <sstream>
#include "resource.h"
//WindowClass Stuff
Window::WindowClass Window::WindowClass::wndClass;

const char* Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}

Window::WindowClass::WindowClass() noexcept
	:
	hInst(GetModuleHandle(nullptr))
{
	//register window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);//structure size
	wc.style = CS_OWNDC;//can spawn multiple window for application
	wc.lpfnWndProc = HandleMsgSetup;// handle all the message
	wc.cbClsExtra = 0;//The number of extra bytes to allocate following the window-class structure. The system initializes the bytes to zero.
	wc.cbWndExtra = 0;//The number of extra bytes to allocate following the window instance. The system initializes the bytes to zero. 
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(
		hInst,MAKEINTRESOURCE(IDI_ICON1),
		IMAGE_ICON,16,16,0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = static_cast<HICON>(
		LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1),
		IMAGE_ICON, 16, 16, 0));;
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, GetInstance());
}

Window::Window(int width, int height, const char* name)
{
	// calculate window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	if (FAILED(AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE)))
	{
		throw CHWND_LAST_EXCEPT();
	}
	// create window & get hWnd
	hWnd = CreateWindow(
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, 
		wr.right - wr.left, 
		wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);
	// check for error
	if (hWnd == nullptr)
	{
		throw CHWND_LAST_EXCEPT();
	}
	// show window
	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		// forward message to window class handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// retrieve ptr to window class
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window class handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT  Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0; //using destructor for destory window
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Window::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
	:
	ChiliException(line,file),
	hr(hr)
{
}

const char* Window::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatbuffer = oss.str();
	return whatbuffer.c_str();
}

const char* Window::Exception::GetType() const noexcept
{
	return "chili Window Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	// windows will allocate memory for err string and make our pointer point to it
	//FormatMessage Formats a message string
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);
	// 0 string length returned indicates a failure
	if (nMsgLen == 0)
	{
		return "Unidentified error code";
	}
	// copy error string from windows-allocated buffer to std::string
	std::string errorString = pMsgBuf;
	// free windows buffer
	LocalFree(pMsgBuf);
	return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}
