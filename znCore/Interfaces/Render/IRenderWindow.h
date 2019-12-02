#pragma once

// FORWARD BEGIN
ZN_INTERFACE IRenderDevice;
ZN_INTERFACE IRenderTarget;
class Viewport;
// FORWARD END

ZN_INTERFACE OW_ENGINE_API IRenderWindow
{
	virtual void                                            ShowWindow() = 0; // Show this window if it is hidden.
	virtual void                                            HideWindow() = 0; // Hide the window. The window will not be destroyed and can be  shown again using the ShowWindow() function.
	virtual void                                            CloseWindow() = 0; // Destroy and close the window.

	virtual void                                            SetMousePosition(vec2 _position) = 0;

	virtual int                                             GetWindowWidth() const = 0;
	virtual int                                             GetWindowHeight() const = 0;
	virtual glm::ivec2                                      GetWindowSize() const = 0;
	virtual bool                                            IsVSync() const = 0;
	virtual HWND                                            GetHWnd() const = 0;

	virtual void											Present() = 0;

	virtual std::shared_ptr<IRenderDevice>                  GetRenderDevice() const = 0;
	virtual std::shared_ptr<IRenderTarget>                  GetRenderTarget() const = 0;
	virtual const Viewport*                                 GetViewport() const = 0;

	virtual LRESULT											WndProc(HWND, UINT, WPARAM, LPARAM) = 0;
};

ZN_INTERFACE OW_ENGINE_API IRenderWindowEvents
{
	virtual Event&				Initialize() = 0;
	virtual      void OnInitialize(EventArgs& e) = 0;
	virtual UpdateEvent&		Update() = 0;
	virtual      void OnUpdate(UpdateEventArgs& e) = 0;
	virtual RenderEvent&        PreRender() = 0;
	virtual      void OnPreRender(RenderEventArgs& e) = 0;
	virtual RenderEvent&        Render() = 0;
	virtual      void OnRender(RenderEventArgs& e) = 0;
	virtual RenderEvent&        PostRender() = 0;
	virtual      void OnPostRender(RenderEventArgs& e) = 0;
	virtual RenderEvent&        RenderUI() = 0;
	virtual      void OnRenderUI(RenderEventArgs& e) = 0;
	virtual Event&				Terminate() = 0;
	virtual      void OnTerminate(EventArgs& e) = 0;
	// Window events
	virtual Event&				InputFocus() = 0; // Window gets input focus
	virtual      void OnInputFocus(EventArgs& e) = 0;
	virtual Event&				InputBlur() = 0;  // Window loses input focus
	virtual      void OnInputBlur(EventArgs& e) = 0;
	virtual Event&				Minimize() = 0;   // Window is minimized.
	virtual      void OnMinimize(EventArgs& e) = 0;
	virtual Event&				Restore() = 0;    // Window is restored.
	virtual      void OnRestore(EventArgs& e) = 0;
	virtual ResizeEvent&        Resize() = 0;
	virtual      void OnResize(ResizeEventArgs& e) = 0;
	virtual Event&				Expose() = 0;
	virtual      void OnExpose(EventArgs& e) = 0;
	// Window is closing
	virtual WindowCloseEvent&   Close() = 0;
	virtual      void OnClose(WindowCloseEventArgs& e) = 0;
	// Keyboard events
	virtual KeyboardEvent&       KeyPressed() = 0;
	virtual       void OnKeyPressed(KeyEventArgs& e) = 0;
	virtual KeyboardEvent&       KeyReleased() = 0;
	virtual      void OnKeyReleased(KeyEventArgs& e) = 0;
	virtual Event&               KeyboardFocus() = 0;
	virtual      void OnKeyboardFocus(EventArgs& e) = 0;
	virtual Event&               KeyboardBlur() = 0;
	virtual      void OnKeyboardBlur(EventArgs& e) = 0;
	// Mouse events
	virtual MouseMotionEvent&   MouseMoved() = 0;
	virtual      void OnMouseMoved(MouseMotionEventArgs& e) = 0;
	virtual MouseButtonEvent&   MouseButtonPressed() = 0;
	virtual      void OnMouseButtonPressed(MouseButtonEventArgs& e) = 0;
	virtual MouseButtonEvent&   MouseButtonReleased() = 0;
	virtual      void OnMouseButtonReleased(MouseButtonEventArgs& e) = 0;
	virtual MouseWheelEvent&    MouseWheel() = 0;
	virtual      void OnMouseWheel(MouseWheelEventArgs& e) = 0;
	virtual Event&              MouseLeave() = 0;
	virtual      void OnMouseLeave(EventArgs& e) = 0;
	virtual Event&              MouseFocus() = 0;
	virtual      void OnMouseFocus(EventArgs& e) = 0;
	virtual Event&              MouseBlur() = 0;
	virtual      void OnMouseBlur(EventArgs& e) = 0;
};