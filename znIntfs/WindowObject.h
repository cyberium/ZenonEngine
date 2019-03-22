#pragma once

struct IWindowObject
{
	virtual ~IWindowObject() {}

	virtual std::string GetWindowName() = 0;
	virtual long GetWindowWidth() = 0;         // include borders
	virtual long GetWindowHeight() = 0;        // include borders
	virtual HWND GetHWnd() = 0;

	virtual HDC BeginPaint(LPPAINTSTRUCT PaintStruct) = 0;
	virtual BOOL EndPaint(LPPAINTSTRUCT PaintStruct) = 0;

	virtual BOOL GetClientRect(LPRECT Rect) = 0;
	virtual BOOL ClientToScreen(LPPOINT Point) = 0;
	virtual BOOL ScreenToClient(LPPOINT Point) = 0;

	virtual BOOL BringWindowToTop() = 0;
	virtual BOOL ShowWindow(int nCmdShow) = 0;
	virtual BOOL UpdateWindow() = 0;
	virtual BOOL DestroyWindow() = 0;
};


struct IWindowObjectEx
{
	virtual ~IWindowObjectEx() {}

	virtual void SetWindowHandle(HWND HWnd) = 0;
	virtual void RegisterWindowClass(HINSTANCE HInstance) = 0;
	virtual void UnregisterWindowClass() = 0;
	virtual HWND CreateWindowInstance(int nWidth, int nHeight) = 0;
	virtual void DestroyWindowInstance() = 0;
};