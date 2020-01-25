#pragma once

// FORWARD BEGIN
ZN_INTERFACE IRenderDevice;
ZN_INTERFACE IRenderTarget;
ZN_INTERFACE IWindowObject;
class Viewport;
// FORWARD END

ZN_INTERFACE ZN_API IRenderWindow
{
	virtual ~IRenderWindow() {}

	virtual size_t GetWindowWidth() const = 0;
	virtual size_t GetWindowHeight() const = 0;
	virtual glm::ivec2 GetWindowSize() const = 0;
	virtual bool IsVSync() const = 0;

	virtual void Present() = 0;

	virtual IRenderDevice* GetRenderDevice() const = 0;
	virtual IRenderTarget* GetRenderTarget() const = 0;
	virtual IWindowObject* GetWindowObject() const = 0;
	virtual const Viewport* GetViewport() const = 0;
};

ZN_INTERFACE ZN_API IRenderWindowEvents
{
	virtual ~IRenderWindowEvents() {}

	virtual UpdateEvent& Update() = 0;
	virtual RenderEvent& PreRender() = 0;
	virtual RenderEvent& Render() = 0;
	virtual RenderEvent& PostRender() = 0;
	virtual RenderEvent& RenderUI() = 0;
};