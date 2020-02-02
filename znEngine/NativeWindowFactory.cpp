#include "stdafx.h"

// General
#include "NativeWindowFactory.h"

// Additional
#include "NativeWindow_WindowsSpecific.h"

namespace
{
	const wchar_t* cZenonWindowClassName = L"ZenonEngineWindowClass";

	LRESULT CALLBACK ZenonWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (message == WM_CREATE)
		{

		}
		else if (message == WM_CLOSE)
		{
			Log::Warn("WndProc: WM_CLOSE.");

			PostQuitMessage(0);

			return DefWindowProc(hwnd, message, wParam, lParam);
		}
		else if (message == WM_DESTROY)
		{
			Log::Warn("WndProc: WM_DESTROY.");

			return DefWindowProc(hwnd, message, wParam, lParam);
		}


		INativeWindow_WindowsSpecific* nativeWindow = (INativeWindow_WindowsSpecific*)::GetWindowLongPtrW(hwnd, GWLP_USERDATA);
		if (nativeWindow != nullptr)
		{
			return nativeWindow->Windows_ProcessMessage(hwnd, message, wParam, lParam);
		}

		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}



CNativeWindowFactory::CNativeWindowFactory(IApplication_WindowsSpecific * Application)
	: m_Application(Application)
{
	//
	// Register window class
	//
	HINSTANCE hDll;
	hDll = LoadLibrary(L"SHELL32.dll");

	WNDCLASSEX renderWindowClass = { 0 };
	renderWindowClass.cbSize = sizeof(WNDCLASSEX);
	renderWindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	renderWindowClass.lpfnWndProc = &ZenonWndProc;
	renderWindowClass.cbClsExtra = 0;
	renderWindowClass.cbWndExtra = 0;
	renderWindowClass.hInstance = m_Application->GetHInstance();
	renderWindowClass.hIcon = LoadIcon(hDll, MAKEINTRESOURCE(2));
	renderWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	renderWindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	renderWindowClass.lpszMenuName = NULL;
	renderWindowClass.lpszClassName = cZenonWindowClassName;
	renderWindowClass.hIconSm = LoadIcon(hDll, MAKEINTRESOURCE(2));

	if (::RegisterClassExW(&renderWindowClass) == FALSE)
		_ASSERT_EXPR(false, "CNativeWindowFactory: Failed to register the render window class.");
}

CNativeWindowFactory::~CNativeWindowFactory()
{
	if (::UnregisterClassW(cZenonWindowClassName, m_Application->GetHInstance()) == FALSE)
		_ASSERT_EXPR(false, "CNativeWindowFactory: Failed to unregister render window class.");
}


//
// IWindowCreator
//
std::unique_ptr<INativeWindow> CNativeWindowFactory::CreateWindowInstance(LPCWSTR WindowName, LONG Width, LONG Height)
{
	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

	RECT windowRect = { 0, 0, Width, Height };
	if (::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE) == FALSE)
		_ASSERT_EXPR(false, "CNativeWindowFactory: Failed to AdjustWindowRect.");

	int windowWidth = windowRect.right - windowRect.left;
	int windowHeight = windowRect.bottom - windowRect.top;

	int windowX = (screenWidth - windowWidth) / 2;
	int windowY = (screenHeight - windowHeight) / 2;

	HWND hWnd = CreateWindowExW
	(
		NULL,
		cZenonWindowClassName,
		WindowName,
		WS_OVERLAPPEDWINDOW,
		windowX, windowY, windowWidth, windowHeight,
		NULL,
		NULL,
		m_Application->GetHInstance(),
		NULL
	);

	if (hWnd == NULL)
		_ASSERT_EXPR(false, L"CNativeWindowFactory: Failed to create render window.");

	if (::ShowWindow(hWnd, SW_SHOWDEFAULT) == FALSE)
	{
	//	_ASSERT_EXPR(false, L"CNativeWindowFactory: Failed to show window.");
	}

	if (::BringWindowToTop(hWnd))
	{
	//	_ASSERT_EXPR(false, "CNativeWindowFactory: Failed to bring window to top.");
	}

	std::unique_ptr<INativeWindow> nativeWindow = std::make_unique<CNativeWindow_WindowsSpecific>(hWnd);
	SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)(dynamic_cast<INativeWindow_WindowsSpecific*>(nativeWindow.get())));
    return std::move(nativeWindow);
}
