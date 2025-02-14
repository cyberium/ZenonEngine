#pragma once

class RenderWindowOGL 
	: public RenderWindowBase
{
public:
	                                                RenderWindowOGL(IRenderDevice* RenderDevice, IznNativeWindow * WindowObject, bool vSync);
	virtual                                         ~RenderWindowOGL();

	void                                            Present() override final;

protected:
    // Engine events
	virtual void                                    OnPreRender(RenderEventArgs& e) override;

protected:
    void                                            CreateSwapChain() override final;
	void                                            ResizeSwapChainBuffers(uint32_t width, uint32_t height) override final;

private:
	HDC                                             m_HDC;
};