#pragma once

class ZN_API CNativeWindow_WindowsSpecific
	: public IznNativeWindow
	, public IznNativeWindow_WindowsSpecific
	, public INativeWindow_WindowsSpecificEx
{
public:
	CNativeWindow_WindowsSpecific(HWND HWnd);
	virtual ~CNativeWindow_WindowsSpecific();

	// IznNativeWindow
	void SetWindowTitle(const std::string& WindowName) override;
	std::string GetWindowTitle() const override;
	size_t GetWindowWidth() const override;
	size_t GetWindowHeight() const override;
	void SetCursorPosition(const glm::ivec2& CursorPosition) override;
	glm::ivec2 GetCursorPosition() const override;
	void ShowCursor() override;
	void HideCursor() override;
	void Close() override;
	void SetEventsListener(IznNativeWindowEventListener* WindowEventsListener) override;
	void ResetEventsListener() override;

	// IznNativeWindow_WindowsSpecific
	HWND GetHWnd() const override;

	// INativeWindow_WindowsSpecificEx
	LRESULT ProcessMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;

private:
	// For mouse events
	glm::ivec2          m_PreviousMousePosition;  // Used to compute relative mouse motion in this window.
	bool                m_InClientRect;           // This is true when the mouse is inside the window's client rect.
	bool                m_IsMouseTracking;        // Used to capture mouse enter/leave events.

	// For keyboard events
	bool                m_bHasKeyboardFocus;      // This is set to true when the window receives keyboard focus.

private:
	HWND				m_HWnd;
	IznNativeWindowEventListener* m_EventListener;
};


