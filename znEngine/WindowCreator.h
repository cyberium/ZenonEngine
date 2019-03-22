#pragma once

class CWindowObject : 
	public IWindowObject,
	public IWindowObjectEx
{
public:
	CWindowObject();
	virtual ~CWindowObject();

	// IWindowObject
	std::string GetWindowName() override;
	long GetWindowWidth() override;
	long GetWindowHeight() override;
	HWND GetHWnd() override;

	BOOL BringWindowToTop();
	HDC  BeginPaint(LPPAINTSTRUCT PaintStruct);
	BOOL EndPaint(LPPAINTSTRUCT PaintStruct);

	BOOL GetClientRect(LPRECT Rect);
	BOOL ClientToScreen(LPPOINT Point);
	BOOL ScreenToClient(LPPOINT Point);

	BOOL ShowWindow(int nCmdShow);
	BOOL UpdateWindow();
	BOOL DestroyWindow();

	// IWindowObjectEx
	void SetWindowHandle(HWND HWnd);

	void RegisterWindowClass(HINSTANCE HInstance);
	void UnregisterWindowClass();
	HWND CreateWindowInstance(int nWidth, int nHeight) ;
	void DestroyWindowInstance();

private:
	// IWindowObject
	HWND      m_HWnd;

	// IWindowObjectEx
	HINSTANCE m_HInstance;
};