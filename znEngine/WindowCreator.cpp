#include "stdafx.h"

// General
#include "WindowCreator.h"

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



HWND CWindowObject::CreateWindowInstance(IApplication * Application, IWindowClassRegistrator * WindowClassRegistrator, LPCWSTR WindowName, LONG Width, LONG Height)
{
	_ASSERT(m_HWnd == NULL);

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
		WindowClassRegistrator->GetWindowClassName(),
		WindowName,
		WS_OVERLAPPEDWINDOW,
		windowX, windowY, windowWidth, windowHeight,
		NULL,
		NULL,
		Application->GetHINSTANCE(),
		NULL
	);

	SetWindowLongPtr(m_HWnd, GWLP_USERDATA, (LONG_PTR)dynamic_cast<IApplicationEvents*>(Application));

	if (m_HWnd == NULL)
	{
		_ASSERT_EXPR(false, "CWindowObject: Failed to create render window.");
	}

    return m_HWnd;
}

void CWindowObject::DestroyWindowInstance()
{
	_ASSERT(m_HWnd != NULL);

	::DestroyWindow(m_HWnd);
}