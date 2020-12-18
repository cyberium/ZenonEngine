#pragma once

class ZN_API CNativeWindowProxy_PlatformWindows
	: public CNativeWindowProxyBase
	, public IznNativeWindow_WindowsSpecific
{
public:
	CNativeWindowProxy_PlatformWindows(IznNativeWindow& SourceNativeWindow);
	virtual ~CNativeWindowProxy_PlatformWindows();

	// IznNativeWindow_WindowsSpecific
	HWND GetHWnd() const override;
};


