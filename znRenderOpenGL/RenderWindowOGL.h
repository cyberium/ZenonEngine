#pragma once

class TextureOGL;
class RenderTargetOGL;

class RenderWindowOGL : public RenderWindow
{
public:
	typedef RenderWindow base;

	RenderWindowOGL(std::shared_ptr<RenderDeviceOGL> device, IWindowObject * WindowObject, bool vSync);
	virtual ~RenderWindowOGL();

	virtual void Present();

	virtual std::shared_ptr<IRenderTarget> GetRenderTarget();

protected:
    virtual void CreateSwapChain();

    // Engine events
	virtual void OnPreRender(RenderEventArgs& e) override;
	virtual void OnPostRender(RenderEventArgs& e) override;

    // Window events
	virtual void OnResize(ResizeEventArgs& e) override;

	virtual void ResizeSwapChainBuffers(uint32_t width, uint32_t height);

private:
	HDC m_HDC;

private:
	std::shared_ptr<RenderTargetOGL> m_RenderTarget;

	// If the window has to be resized, delay the resizing of the swap chain until the prerender function.
	bool m_bResizePending;

	//--
	std::weak_ptr<RenderDeviceOGL> m_RenderDevice;
};