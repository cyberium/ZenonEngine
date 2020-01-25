#pragma once

class ZN_API CWindowHandleWrapper
	: public IWindowObject
	, public IWindowEvents
	, public Object
{
public:
	CWindowHandleWrapper(HWND HWnd);
	virtual ~CWindowHandleWrapper();

	LRESULT WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	//
	// IWindowObject
	//

	std::string GetWindowName() override;
	long GetWindowWidth() override;
	long GetWindowHeight() override;
	HWND GetHWnd() override;
	void SetCursorPosition(const glm::ivec2& CursorPosition) override;
	glm::ivec2 GetCursorPosition() const override;
	void ShowCursor() override;
	void HideCursor() override;
	

	//
	// IWindowEvents
	//

	// Window events
	Event&				InputFocus(); // Window gets input focus
	Event&				InputBlur();  // Window loses input focus
	Event&				Minimize();   // Window is minimized.
	Event&				Restore();    // Window is restored.
	ResizeEvent&        Resize();

	// Window is closing
	WindowCloseEvent&   WindowClose();

	// Keyboard events
	KeyboardEvent&      KeyPressed();
	KeyboardEvent&      KeyReleased();
	Event&              KeyboardFocus();
	Event&              KeyboardBlur();

	// Mouse events
	MouseMotionEvent&   MouseMoved();
	MouseButtonEvent&   MouseButtonPressed();
	MouseButtonEvent&   MouseButtonReleased();
	MouseWheelEvent&    MouseWheel();
	Event&              MouseLeave();
	Event&              MouseFocus();
	Event&              MouseBlur();

private:
	// Window events
	Event				m_InputFocus; // Window gets input focus
	Event				m_InputBlur;  // Window loses input focus
	Event				m_Minimize;   // Window is minimized.
	Event				m_Restore;    // Window is restored.
	ResizeEvent         m_Resize;
	Event				m_Expose;

	// Window is closing
	WindowCloseEvent    m_Close;

	// Keyboard events
	KeyboardEvent       m_KeyPressed;
	KeyboardEvent       m_KeyReleased;
	Event               m_KeyboardFocus;
	Event               m_KeyboardBlur;

	// Mouse events
	MouseMotionEvent    m_MouseMoved;
	MouseButtonEvent    m_MouseButtonPressed;
	MouseButtonEvent    m_MouseButtonReleased;
	MouseWheelEvent     m_MouseWheel;
	Event               m_MouseLeave;
	Event               m_MouseFocus;
	Event               m_MouseBlur;

private:
	// For mouse events
	glm::ivec2          m_PreviousMousePosition;  // Used to compute relative mouse motion in this window.
	bool                m_InClientRect;           // This is true when the mouse is inside the window's client rect.
	bool                m_IsMouseTracking;        // Used to capture mouse enter/leave events.

	// For keyboard events
	bool                m_bHasKeyboardFocus;      // This is set to true when the window receives keyboard focus.

private:
	HWND m_HWnd;
};