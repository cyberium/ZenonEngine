#pragma once

class TextureOGL;
class RenderTargetOGL;

class RenderWindowOGL : public RenderWindow
{
public:
	typedef RenderWindow base;

	RenderWindowOGL(HWND hWnd, std::shared_ptr<RenderDeviceOGL> device, cstring windowName, int windowWidth, int windowHeight, bool vSync);
	virtual ~RenderWindowOGL();

	virtual void Present();

	virtual std::shared_ptr<IRenderTarget> GetRenderTarget();

protected:
	virtual void OnPreRender(Render3DEventArgs& e);
	virtual void OnPostRender(Render3DEventArgs& e);

	virtual void OnMouseMoved(MouseMotionEventArgs& e);
	virtual void OnMouseLeave(EventArgs& e);

	virtual void OnResize(ResizeEventArgs& e);

	virtual void OnTerminate(EventArgs& e);

	virtual void ResizeSwapChainBuffers(uint32_t width, uint32_t height);

private:
	HDC m_HDC;

private:
	bool m_bIsMouseTracking; // Used to capture mouse enter/leave events.

	std::shared_ptr<RenderTargetOGL> m_RenderTarget;

	// If the window has to be resized, delay the resizing of the swap chain until the prerender function.
	bool m_bResizePending;

	//--
	std::weak_ptr<RenderDeviceOGL> m_RenderDevice;
};