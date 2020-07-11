#pragma once

// FORWARD BEGIN
ZN_INTERFACE IRenderDevice;
ZN_INTERFACE IRenderTarget;
ZN_INTERFACE INativeWindow;
class Viewport;
// FORWARD END

ZN_INTERFACE ZN_API IRenderWindow
	: public INativeWindow
	, public std::enable_shared_from_this<IRenderWindow>
{
	virtual ~IRenderWindow() {}

	virtual bool IsVSync() const = 0;
	virtual void Present() = 0;

	virtual const std::shared_ptr<IRenderTarget>& GetRenderTarget() const = 0;
	virtual const Viewport& GetViewport() const = 0;
};

ZN_INTERFACE ZN_API IRenderWindowEvents
{
	virtual ~IRenderWindowEvents() {}

	virtual UpdateEvent&        Update() = 0;
	virtual RenderEvent&        PreRender() = 0;
	virtual RenderEvent&        Render() = 0;
	virtual RenderEvent&        PostRender() = 0;
	virtual RenderEvent&        RenderUI() = 0;

	// Window events
	virtual Event&				WindowInputFocus() = 0; // Window gets input focus
	virtual Event&				WindowInputBlur() = 0;  // Window loses input focus
	virtual Event&				WindowMinimize() = 0;   // Window is minimized.
	virtual Event&				WindowRestore() = 0;    // Window is restored.
	virtual ResizeEvent&		WindowResize() = 0;
	virtual WindowCloseEvent&	WindowClose() = 0;

	// Keyboard events
	virtual KeyboardEvent&		WindowKeyPressed() = 0;
	virtual KeyboardEvent&		WindowKeyReleased() = 0;
	virtual Event&				WindowKeyboardFocus() = 0;
	virtual Event&				WindowKeyboardBlur() = 0;

	// Mouse events
	virtual MouseMotionEvent&   WindowMouseMoved() = 0;
	virtual MouseButtonEvent&   WindowMouseButtonPressed() = 0;
	virtual MouseButtonEvent&   WindowMouseButtonReleased() = 0;
	virtual MouseWheelEvent&    WindowMouseWheel() = 0;
	virtual Event&              WindowMouseLeave() = 0;
	virtual Event&              WindowMouseFocus() = 0;
	virtual Event&              WindowMouseBlur() = 0;
};