#pragma once

class ZN_API RenderWindowBase 
	: public IRenderWindow
	, public IRenderWindowEvents
	, public IApplicationEventsConnection
	, public Object
	, public std::enable_shared_from_this<IRenderWindow>
{
public:
	RenderWindowBase(std::shared_ptr<IRenderDevice> RenderDevice, IWindowObject * WindowObject, bool vSync = false);
	virtual ~RenderWindowBase();

	// IRenderWindow
	int                                             GetWindowWidth() const;
	int                                             GetWindowHeight() const;
	glm::ivec2                                      GetWindowSize() const;
	bool                                            IsVSync() const;

	virtual void                                    Present() = 0;

    std::shared_ptr<IRenderDevice>                  GetRenderDevice() const;
	std::shared_ptr<IRenderTarget>                  GetRenderTarget() const;
	IWindowObject*                                  GetWindowObject() const;
	const Viewport*									GetViewport() const;

	// IRenderWindowEvents
	UpdateEvent& Update() override;
	RenderEvent& PreRender() override;
	RenderEvent& Render() override;
	RenderEvent& PostRender() override;
	RenderEvent& RenderUI() override;

	virtual void OnUpdate(UpdateEventArgs& e);
	virtual void OnPreRender(RenderEventArgs& e);
	virtual void OnRender(RenderEventArgs& e);
	virtual void OnPostRender(RenderEventArgs& e);
	virtual void OnRenderUI(RenderEventArgs& e);

	// Window events
	virtual void OnResize(ResizeEventArgs& e);

	// Window is closing
	virtual void OnClose(WindowCloseEventArgs& e);

	// IApplicationEventsConnection
	void											Connect(IApplicationEvents* ApplicationEvents) override;
	void											Disconnect(IApplicationEvents* ApplicationEvents) override;

protected:
    virtual void                                    CreateSwapChain();
    virtual void                                    ResizeSwapChainBuffers(uint32_t width, uint32_t height) = 0;

private:
	Viewport                                        m_Viewport;
	IWindowObject *                                 m_WindowObject;

    std::weak_ptr<IRenderDevice>                    m_RenderDevice;
    std::shared_ptr<IRenderTarget>                  m_RenderTarget;

	bool                                            m_vSync;
	
    bool                                            m_bResizePending;  



private:
	// IRenderWindowEvents
	UpdateEvent			m_Update;
	RenderEvent         m_PreRender;
	RenderEvent         m_Render;
	RenderEvent         m_PostRender;
	RenderEvent         m_RenderUI;

private: // IApplicationEventsConnection
	Delegate<UpdateEventArgs>::FunctionDecl  m_UpdateConnection;

private: // IWindowEventsConnection
	Delegate<ResizeEventArgs>::FunctionDecl  m_ResizeConnection;
	Delegate<WindowCloseEventArgs>::FunctionDecl  m_WindowCloseConnection;
};
