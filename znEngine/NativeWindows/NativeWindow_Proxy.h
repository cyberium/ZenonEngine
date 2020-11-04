#pragma once

class ZN_API CNativeWindow_Proxy
	: public IznNativeWindow
	, public IznNativeWindow_WindowsSpecific
	, public IznNativeWindowEventListener
{
public:
	CNativeWindow_Proxy(IznNativeWindow& SourceNativeWindow);
	virtual ~CNativeWindow_Proxy();

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

	// IznNativeWindowEventListener
	void OnWindowInputFocus(EventArgs& Args) override; // Window gets input focus
	void OnWindowInputBlur(EventArgs& Args) override;  // Window loses input focus
	void OnWindowMinimize(EventArgs& Args) override;   // Window is minimized.
	void OnWindowRestore(EventArgs& Args) override;    // Window is restored.
	void OnWindowResize(ResizeEventArgs& Args) override;
	void OnWindowClose(WindowCloseEventArgs& Args) override;
	bool OnWindowKeyPressed(KeyEventArgs& Args) override;
	void OnWindowKeyReleased(KeyEventArgs& Args) override;
	void OnWindowKeyboardFocus(EventArgs& Args) override;
	void OnWindowKeyboardBlur(EventArgs& Args) override;
	void OnWindowMouseMoved(MouseMotionEventArgs& Args) override;
	bool OnWindowMouseButtonPressed(MouseButtonEventArgs& Args) override;
	void OnWindowMouseButtonReleased(MouseButtonEventArgs& Args) override;
	bool OnWindowMouseWheel(MouseWheelEventArgs& Args) override;
	void OnWindowMouseLeave(EventArgs& Args) override;
	void OnWindowMouseFocus(EventArgs& Args) override;
	void OnWindowMouseBlur(EventArgs& Args) override;

protected:
	IznNativeWindowEventListener* GetMyEventsListener();

private:
	IznNativeWindowEventListener* m_EventListener;
	IznNativeWindow& m_SourceNativeWindow;
};


