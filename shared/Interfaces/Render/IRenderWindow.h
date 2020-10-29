#pragma once

// FORWARD BEGIN
ZN_INTERFACE IRenderDevice;
ZN_INTERFACE IRenderTarget;
ZN_INTERFACE INativeWindow;
class Viewport;
// FORWARD END



ZN_INTERFACE ZN_API IRenderWindowEventListener
{
	virtual ~IRenderWindowEventListener() {}

	virtual void OnUpdate(UpdateEventArgs& e) = 0;
	virtual void OnPreRender(RenderEventArgs& e) = 0;
	virtual void OnRender(RenderEventArgs& e) = 0;
	virtual void OnPostRender(RenderEventArgs& e) = 0;
	virtual void OnRenderUI(RenderEventArgs& e) = 0;
};



ZN_INTERFACE ZN_API IRenderWindow
	: public INativeWindow
	//, public std::enable_shared_from_this<IRenderWindow>
{
	virtual ~IRenderWindow() {}

	virtual bool IsVSync() const = 0;
	virtual void Present() = 0;

	virtual void SetRenderWindowEventListener(std::shared_ptr<IRenderWindowEventListener> RenderWindowEventListener) = 0;
	virtual void SetNativeWindowEventListener(std::shared_ptr<INativeWindowEventListener> NativeWindowEventListener) = 0;
	virtual const std::shared_ptr<IRenderTarget>& GetRenderTarget() const = 0;
	virtual const Viewport& GetViewport() const = 0;

	virtual float GetUpdateDeltaTime() const = 0;
	virtual float GetPreRenderDeltaTime() const = 0;
	virtual float GetRenderDeltaTime() const = 0;
	virtual float GetPostRenderDeltaTime() const = 0;
	virtual float GetRenderUIDeltaTime() const = 0;
	virtual float GetSummaDeltaTime() const = 0;
};
