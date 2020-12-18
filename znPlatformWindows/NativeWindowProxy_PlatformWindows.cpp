#include "stdafx.h"

// General
#include "NativeWindowProxy_PlatformWindows.h"


CNativeWindowProxy_PlatformWindows::CNativeWindowProxy_PlatformWindows(IznNativeWindow& SourceNativeWindow)
	: CNativeWindowProxyBase(SourceNativeWindow)
{}

CNativeWindowProxy_PlatformWindows::~CNativeWindowProxy_PlatformWindows()
{}



//
// IznNativeWindow_WindowsSpecific
//
HWND CNativeWindowProxy_PlatformWindows::GetHWnd() const
{
	return dynamic_cast<IznNativeWindow_WindowsSpecific&>(GetSourceNativeWindow()).GetHWnd();
}
