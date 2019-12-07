#include "stdafx.h"

// General
#include "WindowClassRegistrator.h"

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


CWindowClassRegistratorObject::CWindowClassRegistratorObject(IApplication * Application)
{
	m_HInstance = Application->GetHINSTANCE();
}

CWindowClassRegistratorObject::~CWindowClassRegistratorObject()
{}

void CWindowClassRegistratorObject::RegisterWindowClass(LPCWSTR WindowClassName)
{
	_ASSERT(m_HInstance != NULL);
	
	m_ClassName = WindowClassName;

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
	renderWindowClass.lpszClassName = m_ClassName.c_str();
	renderWindowClass.hIconSm = LoadIcon(hDll, MAKEINTRESOURCE(2));

	if (RegisterClassEx(&renderWindowClass) == FALSE)
	{
		_ASSERT_EXPR(false, "CWindowClassRegistratorObject: Failed to register the render window class.");
	}
}

void CWindowClassRegistratorObject::UnregisterWindowClass()
{
	_ASSERT(m_HInstance != NULL);

	if (UnregisterClass(m_ClassName.c_str(), m_HInstance) == FALSE)
	{
		_ASSERT_EXPR(false, "CWindowClassRegistratorObject: Failed to unregister render window class");
	}
}

LPCWSTR CWindowClassRegistratorObject::GetWindowClassName() const
{
	return m_ClassName.c_str();
}