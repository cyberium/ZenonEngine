#include "stdafx.h"

// General
#include "WindowCreator.h"

// Additional
#include "Application.h"

const char*    c_RenderWindow_ClassNameA = "RenderWindowClass";
const wchar_t* c_RenderWindow_ClassNameW = L"RenderWindowClass";

#ifdef UNICODE
#define c_RenderWindow_ClassName  c_RenderWindow_ClassNameW
#else
#define c_RenderWindow_ClassName  c_RenderWindow_ClassNameA
#endif // !UNICODE

CWindowObject::CWindowObject()
	: m_HWnd(NULL)
	, m_HInstance(NULL)
{
}


CWindowObject::~CWindowObject()
{
}



//
// IWindowObject
//
std::string CWindowObject::GetWindowName()
{
	std::wstring title(GetWindowTextLength(m_HWnd) + 1, L'\0');
	GetWindowText(m_HWnd, &title[0], title.size());
	return ConvertString(title);
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

void CWindowObject::RegisterWindowClass(HINSTANCE HInstance)
{
	_ASSERT(m_HInstance == NULL);
	m_HInstance = HInstance;

	HINSTANCE hDll;
	hDll = LoadLibrary(L"SHELL32.dll");

	// Register a window class for creating our render windows with.
	WNDCLASSEX renderWindowClass;
	renderWindowClass.cbSize = sizeof(WNDCLASSEX);
	renderWindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	renderWindowClass.lpfnWndProc = &Application::WndProc;
	renderWindowClass.cbClsExtra = 0;
	renderWindowClass.cbWndExtra = 0;
	renderWindowClass.hInstance = m_HInstance;
	renderWindowClass.hIcon = LoadIcon(hDll, MAKEINTRESOURCE(2));
	renderWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	renderWindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	renderWindowClass.lpszMenuName = NULL;
	renderWindowClass.lpszClassName = c_RenderWindow_ClassName;
	renderWindowClass.hIconSm = LoadIcon(hDll, MAKEINTRESOURCE(2));

	if (RegisterClassEx(&renderWindowClass) == FALSE)
	{
		fail2("CWindowObject: Failed to register the render window class.");
	}
}

void CWindowObject::UnregisterWindowClass()
{
	_ASSERT(m_HInstance != NULL);

	if (m_HWnd != NULL)
	{
		DestroyWindowInstance();
	}

	if (UnregisterClass(c_RenderWindow_ClassName, m_HInstance) == FALSE)
	{
		fail2("CWindowObject: Failed to unregister render window class");
	}
}

HWND CWindowObject::CreateWindowInstance(int nWidth, int nHeight)
{
	_ASSERT(m_HWnd == NULL);
	_ASSERT(m_HInstance != NULL);

	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

	RECT windowRect = { 0, 0, nWidth, nHeight };
	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	int windowWidth = windowRect.right - windowRect.left;
	int windowHeight = windowRect.bottom - windowRect.top;

	int windowX = (screenWidth - windowWidth) / 2;
	int windowY = (screenHeight - windowHeight) / 2;

	m_HWnd = CreateWindowEx
	(
		NULL,
		c_RenderWindow_ClassName,
		c_RenderWindow_ClassName,
		WS_OVERLAPPEDWINDOW,
		windowX, windowY, windowWidth, windowHeight,
		NULL,
		NULL,
		m_HInstance,
		NULL
	);

	if (m_HWnd == NULL)
	{
		fail2("CWindowObject: Failed to create render window.");
	}

    return m_HWnd;
}

void CWindowObject::DestroyWindowInstance()
{
	_ASSERT(m_HWnd != NULL);
	_ASSERT(m_HInstance != NULL);

	DestroyWindow();
}