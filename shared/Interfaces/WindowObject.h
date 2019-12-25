#pragma once

ZN_INTERFACE ZN_API IWindowObject
{
	virtual ~IWindowObject() {}

	virtual std::string GetWindowName() = 0;
	virtual long GetWindowWidth() = 0;         // include borders
	virtual long GetWindowHeight() = 0;        // include borders
	virtual HWND GetHWnd() = 0;
	virtual void SetCursorPosition(const glm::ivec2& CursorPosition) = 0;
	virtual glm::ivec2 GetCursorPosition() const = 0;
	virtual void ShowCursor() = 0;
	virtual void HideCursor() = 0;
};

ZN_INTERFACE ZN_API IWindowEvents
{
	virtual ~IWindowEvents() {}

	// Window events
	virtual Event&				InputFocus() = 0; // Window gets input focus
	virtual Event&				InputBlur() = 0;  // Window loses input focus
	virtual Event&				Minimize() = 0;   // Window is minimized.
	virtual Event&				Restore() = 0;    // Window is restored.
	virtual ResizeEvent&		Resize() = 0;
	// Window is closing
	virtual WindowCloseEvent&	Close() = 0;
	// Keyboard events
	virtual KeyboardEvent&		KeyPressed() = 0;
	virtual KeyboardEvent&		KeyReleased() = 0;
	virtual Event&				KeyboardFocus() = 0;
	virtual Event&				KeyboardBlur() = 0;
	// Mouse events
	virtual MouseMotionEvent&   MouseMoved() = 0;
	virtual MouseButtonEvent&   MouseButtonPressed() = 0;
	virtual MouseButtonEvent&   MouseButtonReleased() = 0;
	virtual MouseWheelEvent&    MouseWheel() = 0;
	virtual Event&              MouseLeave() = 0;
	virtual Event&              MouseFocus() = 0;
	virtual Event&              MouseBlur() = 0;
};

ZN_INTERFACE ZN_API IWindowEventsConnection
{
	virtual ~IWindowEventsConnection() {}

	virtual void Connect(IWindowEvents* WindowEvents) = 0;
	virtual void Disconnect(IWindowEvents* WindowEvents) = 0;
};

ZN_INTERFACE ZN_API IWindowObjectEx
{
	virtual ~IWindowObjectEx() {}

	virtual void SetWindowHandle(HWND HWnd) = 0;
	virtual HWND CreateWindowInstance(IApplication * Application, LPCWSTR WindowName, LONG Width, LONG Height) = 0;
	virtual void DestroyWindowInstance() = 0;
};
