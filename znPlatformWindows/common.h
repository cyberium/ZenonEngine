#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN 1
#endif

#ifndef NOMINMAX
#   define NOMINMAX
#endif

#ifndef VC_EXTRALEAN
#	define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

#include <windows.h>


ZN_INTERFACE ZN_API IApplication_WindowsSpecific
{
	virtual ~IApplication_WindowsSpecific() {}

	virtual HINSTANCE GetHInstance() const = 0;
};


ZN_INTERFACE ZN_API IznNativeWindow_WindowsSpecific // WINDOWS only but in my engine only Windows avaliable... :(
{
	virtual ~IznNativeWindow_WindowsSpecific() {}

	virtual HWND GetHWnd() const = 0;
};


ZN_INTERFACE ZN_API INativeWindow_WindowsSpecificEx // WINDOWS only but in my engine only Windows avaliable... :(
{
	virtual ~INativeWindow_WindowsSpecificEx() {}

	virtual LRESULT ProcessMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
};