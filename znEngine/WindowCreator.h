#pragma once

class OW_ENGINE_API CWindowObject :
	public IWindowObject
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

	HWND CreateWindowInstance(IApplication * Application, IWindowClassRegistrator * WindowClassRegistrator, LPCWSTR WindowName, LONG Width, LONG Height) ;
	void DestroyWindowInstance();

private:
	// IWindowObject
	HWND      m_HWnd;
};