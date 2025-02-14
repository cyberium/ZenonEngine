#pragma once

// FORWARD BEGIN
ZN_INTERFACE IznNativeWindowEventListener;
// FORWARD END


ZN_INTERFACE ZN_API IznNativeWindow
{
	virtual ~IznNativeWindow() {}

	virtual void SetWindowTitle(const std::string& WindowName) = 0;
	virtual std::string GetWindowTitle() const = 0;
	
	virtual size_t GetWindowWidth() const = 0;         // include borders
	virtual size_t GetWindowHeight() const = 0;        // include borders
	
	virtual void SetCursorPosition(const glm::ivec2& CursorPosition) = 0;
	virtual glm::ivec2 GetCursorPosition() const = 0;
	
	virtual void ShowCursor() = 0;
	virtual void HideCursor() = 0;

	virtual void Close() = 0;

	virtual void SetEventsListener(IznNativeWindowEventListener* WindowEventsListener) = 0;
	virtual void ResetEventsListener() = 0;
};


ZN_INTERFACE ZN_API IznNativeWindowEventListener
{
	virtual ~IznNativeWindowEventListener() {}

	// Window events
	virtual void OnWindowInputFocus(EventArgs& Args) = 0; // Window gets input focus
	virtual void OnWindowInputBlur(EventArgs& Args) = 0;  // Window loses input focus
	virtual void OnWindowMinimize(EventArgs& Args) = 0;   // Window is minimized.
	virtual void OnWindowRestore(EventArgs& Args) = 0;    // Window is restored.
	virtual void OnWindowResize(ResizeEventArgs& Args) = 0;
	virtual void OnWindowClose(WindowCloseEventArgs& Args) = 0;

	// Keyboard events
	virtual bool OnWindowKeyPressed(KeyEventArgs& Args) = 0;
	virtual void OnWindowKeyReleased(KeyEventArgs& Args) = 0;
	virtual void OnWindowKeyboardFocus(EventArgs& Args) = 0;
	virtual void OnWindowKeyboardBlur(EventArgs& Args) = 0;

	// Mouse events
	virtual void OnWindowMouseMoved(MouseMotionEventArgs& Args) = 0;
	virtual bool OnWindowMouseButtonPressed(MouseButtonEventArgs& Args) = 0;
	virtual void OnWindowMouseButtonReleased(MouseButtonEventArgs& Args) = 0;
	virtual bool OnWindowMouseWheel(MouseWheelEventArgs& Args) = 0;
	virtual void OnWindowMouseLeave(EventArgs& Args) = 0;
	virtual void OnWindowMouseFocus(EventArgs& Args) = 0;
	virtual void OnWindowMouseBlur(EventArgs& Args) = 0;
};


ZN_INTERFACE ZN_API IznNativeWindowFactory
{
	virtual ~IznNativeWindowFactory() {}

	virtual std::unique_ptr<IznNativeWindow> CreateWindowInstance(const std::string& WindowName, glm::ivec2 WindowSize) const = 0;
	virtual std::unique_ptr<IznNativeWindow> CreateWindowProxy(IznNativeWindow& SourceNativeWindow) const = 0;
};
