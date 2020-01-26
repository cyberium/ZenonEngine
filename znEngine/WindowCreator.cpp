#include "stdafx.h"

// General
#include "WindowCreator.h"

LRESULT CALLBACK WndProcLink(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_CREATE)
	{

	}
	else if (message == WM_CLOSE)
	{
		Log::Warn("WndProc: WM_CLOSE.");

		/*WindowCloseEventArgs windowCloseEventArgs(this);

		m_Close(windowCloseEventArgs);

		if (windowCloseEventArgs.ConfirmClose)
		{
			::ShowWindow(hwnd, SW_HIDE);
		}*/

		PostQuitMessage(0);

		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	else if (message == WM_DESTROY)
	{
		Log::Warn("WndProc: WM_DESTROY.");

		return DefWindowProc(hwnd, message, wParam, lParam);
	}


	IWindowObject* windowObject = (IWindowObject*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (windowObject != nullptr)
	{
		return windowObject->WndProc(hwnd, message, wParam, lParam);
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}



CWindowCreator::CWindowCreator(IApplication * Application, LPCWSTR WindowName, LONG Width, LONG Height)
	: m_HInstance(NULL)
	, m_HWnd(NULL)
	, m_WindowHandleWrapper(nullptr)

{
	CreateWindowInstance(Application, WindowName, Width, Height);
}

CWindowCreator::~CWindowCreator()
{
	DestroyWindowInstance();
}

const CWindowHandleWrapper* CWindowCreator::GetHandleWrapper() const
{
	return m_WindowHandleWrapper.get();
}



//
// IWindowCreator
//
HWND CWindowCreator::CreateWindowInstance(IApplication * Application, LPCWSTR WindowName, LONG Width, LONG Height)
{
	//
	// Register window class
	//

	m_HInstance = Application->GetHINSTANCE();
	m_WindowClassName = std::wstring(WindowName) + std::wstring(L"Ñlass");

	HINSTANCE hDll;
	hDll = LoadLibrary(L"SHELL32.dll");

	WNDCLASSEX renderWindowClass = { 0 };
	renderWindowClass.cbSize = sizeof(WNDCLASSEX);
	renderWindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	renderWindowClass.lpfnWndProc = &WndProcLink;
	renderWindowClass.cbClsExtra = 0;
	renderWindowClass.cbWndExtra = 0;
	renderWindowClass.hInstance = m_HInstance;
	renderWindowClass.hIcon = LoadIcon(hDll, MAKEINTRESOURCE(2));
	renderWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	renderWindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	renderWindowClass.lpszMenuName = NULL;
	renderWindowClass.lpszClassName = m_WindowClassName.c_str();
	renderWindowClass.hIconSm = LoadIcon(hDll, MAKEINTRESOURCE(2));

	if (::RegisterClassEx(&renderWindowClass) == FALSE)
		_ASSERT_EXPR(false, "CWindowCreator: Failed to register the render window class.");

	//
	// Create window
	//

	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

	RECT windowRect = { 0, 0, Width, Height };
	if (::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE) == FALSE)
		_ASSERT_EXPR(false, "CWindowCreator: Failed to AdjustWindowRect.");

	int windowWidth = windowRect.right - windowRect.left;
	int windowHeight = windowRect.bottom - windowRect.top;

	int windowX = (screenWidth - windowWidth) / 2;
	int windowY = (screenHeight - windowHeight) / 2;

	m_HWnd = CreateWindowEx
	(
		NULL,
		m_WindowClassName.c_str(),
		WindowName,
		WS_OVERLAPPEDWINDOW,
		windowX, windowY, windowWidth, windowHeight,
		NULL,
		NULL,
		m_HInstance,
		NULL
	);

	if (m_HWnd == NULL)
		_ASSERT_EXPR(false, L"CWindowCreator: Failed to create render window.");


	m_WindowHandleWrapper = std::make_unique<CWindowHandleWrapper>(m_HWnd);
	SetWindowLongPtrW(m_HWnd, GWLP_USERDATA, (LONG_PTR)(m_WindowHandleWrapper.get()));

	if (::ShowWindow(m_HWnd, SW_SHOWDEFAULT) == FALSE)
	{
	//	_ASSERT_EXPR(false, L"CWindowCreator: Failed to show window.");
	}

	if (::BringWindowToTop(m_HWnd))
	{
	//	_ASSERT_EXPR(false, "CWindowCreator: Failed to bring window to top.");
	}

    return m_HWnd;
}

void CWindowCreator::DestroyWindowInstance()
{
	_ASSERT(m_HWnd != NULL);

	if (::IsWindow(m_HWnd))
	{
		if (::DestroyWindow(m_HWnd) == FALSE)
			_ASSERT_EXPR(false, "CWindowCreator: Failed to destroy window object.");
	}

	if (::UnregisterClass(m_WindowClassName.c_str(), m_HInstance) == FALSE)
		_ASSERT_EXPR(false, "CWindowCreator: Failed to unregister render window class.");
}