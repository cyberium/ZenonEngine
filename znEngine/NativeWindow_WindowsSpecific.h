#pragma once

class ZN_API CNativeWindow_WindowsSpecific
	: public INativeWindow
	, public INativeWindow_WindowsSpecific
	, public Object
{
public:
	CNativeWindow_WindowsSpecific(HWND HWnd);
	virtual ~CNativeWindow_WindowsSpecific();

	// INativeWindow
	void SetWindowTitle(const std::string& WindowName) override;
	std::string GetWindowTitle() const override;
	size_t GetWindowWidth() const override;
	size_t GetWindowHeight() const override;
	void SetCursorPosition(const glm::ivec2& CursorPosition) override;
	glm::ivec2 GetCursorPosition() const override;
	void ShowCursor() override;
	void HideCursor() override;
	void SetEventsListener(INativeWindowEventListener* WindowEventsListener) override;
	void ResetEventsListener() override;

	// INativeWindow_WindowsSpecific
	HWND GetHWnd() const override;
	LRESULT Windows_ProcessMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;

private:
	// For mouse events
	glm::ivec2          m_PreviousMousePosition;  // Used to compute relative mouse motion in this window.
	bool                m_InClientRect;           // This is true when the mouse is inside the window's client rect.
	bool                m_IsMouseTracking;        // Used to capture mouse enter/leave events.

	// For keyboard events
	bool                m_bHasKeyboardFocus;      // This is set to true when the window receives keyboard focus.

private:
	HWND				m_HWnd;
	INativeWindowEventListener* m_EventListener;
};