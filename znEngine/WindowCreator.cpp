#include "stdafx.h"

// General
#include "WindowCreator.h"



// STATIC
LRESULT CALLBACK WndProcLink(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	IApplicationEvents* me = (IApplicationEvents*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (me)
	{
		return me->WndProc(hwnd, message, wParam, lParam);
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
// STATIC



CWindowObject::CWindowObject()
	: m_HWnd(NULL)
{}

CWindowObject::~CWindowObject()
{
	DestroyWindowInstance();
}



//
// IWindowObject
//
std::string CWindowObject::GetWindowName()
{
	std::wstring title(GetWindowTextLength(m_HWnd) + 1, L'\0');
	GetWindowText(m_HWnd, &title[0], title.size());
	return Resources::ConvertString(title);
}

long CWindowObject::GetWindowWidth()
{
    RECT windowRect;
    ::GetWindowRect(m_HWnd, &windowRect);

    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;

	return windowWidth;
}

long CWindowObject::GetWindowHeight()
{
    RECT windowRect;
    ::GetWindowRect(m_HWnd, &windowRect);

    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;

	return windowHeight;
}

HWND CWindowObject::GetHWnd()
{
	return m_HWnd;
}

BOOL CWindowObject::BringWindowToTop()
{
	return ::BringWindowToTop(m_HWnd);
}

HDC CWindowObject::BeginPaint(LPPAINTSTRUCT PaintStruct)
{
	return ::BeginPaint(m_HWnd, PaintStruct);
}

BOOL CWindowObject::EndPaint(LPPAINTSTRUCT PaintStruct)
{
	return ::EndPaint(m_HWnd, PaintStruct);
}

BOOL CWindowObject::GetClientRect(LPRECT Rect)
{
	return ::GetClientRect(m_HWnd, Rect);
}

BOOL CWindowObject::ClientToScreen(LPPOINT Point)
{
	return ::ClientToScreen(m_HWnd, Point);
}

BOOL CWindowObject::ScreenToClient(LPPOINT Point)
{
	return ::ScreenToClient(m_HWnd, Point);
}

BOOL CWindowObject::ShowWindow(int nCmdShow)
{
	return ::ShowWindow(m_HWnd, nCmdShow);
}

BOOL CWindowObject::UpdateWindow()
{
	return ::UpdateWindow(m_HWnd);
}

BOOL CWindowObject::DestroyWindow()
{
	return ::DestroyWindow(m_HWnd);
}



//
// IWindowObjectEx
//
void CWindowObject::SetWindowHandle(HWND HWnd)
{
	_ASSERT(m_HWnd == NULL);
	m_HWnd = HWnd;
}

HWND CWindowObject::CreateWindowInstance(IApplication * Application, LPCWSTR WindowName, LONG Width, LONG Height)
{
	_ASSERT_EXPR(m_HWnd == NULL, "Window already created.");

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

	if (RegisterClassEx(&renderWindowClass) == FALSE)
	{
		_ASSERT_EXPR(false, "CWindowObject: Failed to register the render window class.");
	}

	//
	// Create window
	//

	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

	RECT windowRect = { 0, 0, Width, Height };
	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	int windowWidth = windowRect.right - windowRect.left;
	int windowHeight = windowRect.bottom - windowRect.top;

	int windowX = (screenWidth - windowWidth) / 2;
	int windowY = (screenHeight - windowHeight) / 2;

	m_HWnd = CreateWindowExW
	(
		NULL,
		m_WindowClassName.c_str(),
		WindowName,
		WS_OVERLAPPEDWINDOW,
		windowX, windowY, windowWidth, windowHeight,
		NULL,
		NULL,
		Application->GetHINSTANCE(),
		NULL
	);

	if (m_HWnd == NULL)
	{
		_ASSERT_EXPR(false, "CWindowObject: Failed to create render window.");
	}

	SetWindowLongPtr(m_HWnd, GWLP_USERDATA, (LONG_PTR)dynamic_cast<IApplicationEvents*>(Application));

    return m_HWnd;
}

void CWindowObject::DestroyWindowInstance()
{
	_ASSERT(m_HWnd != NULL);

	if (::DestroyWindow(m_HWnd) == FALSE)
	{
		_ASSERT_EXPR(false, "CWindowObject: Failed to destroy window object.");
	}

	if (::UnregisterClass(m_WindowClassName.c_str(), m_HInstance) == FALSE)
	{
		_ASSERT_EXPR(false, "CWindowObject: Failed to unregister render window class.");
	}
}