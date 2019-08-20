#include "Window.h"
#include <sstream>
int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	try
	{
		Window wnd(800, 300, "Game Engine");

		MSG msg;
		BOOL gResult;
		while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			// test code
			while (!wnd.mouse.IsEmpty())
			{
				const auto e = wnd.mouse.Read();
				switch (e.GetType())
				{
				case Mouse::Event::Type::Leave:
					wnd.SetTitle("Gone!");
					break;
				case Mouse::Event::Type::Move:
				{
					std::ostringstream oss;
					oss << "Mouse moved to (" << e.GetPosX() << "," << e.GetPosY() << ")";
					wnd.SetTitle(oss.str());
				}
				break;
				}
			}
		}

		// check if GetMessage call itself borked
		if (gResult == -1)
		{
			return -1;
		}

		// wParam here is the value passed to PostQuitMessage
		return msg.wParam;
	}
	catch (const ChiliException& e)
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch(...)
	{
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}