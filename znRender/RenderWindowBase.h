#pragma once

class ZN_API RenderWindowBase 
	: public IRenderWindow
	, public INativeWindowEventListener
	, public IApplicationEventsConnection
{
public:
	RenderWindowBase(INativeWindow& WindowObject, bool vSync);
	virtual ~RenderWindowBase();

	virtual void OnUpdate(UpdateEventArgs& e);
	virtual void OnPreRender(RenderEventArgs& e);
	virtual void OnRender(RenderEventArgs& e);
	virtual void OnPostRender(RenderEventArgs& e);
	virtual void OnRenderUI(RenderEventArgs& e);

	// INativeWindow
	void SetWindowTitle(const std::string& WindowName) override;
	std::string GetWindowTitle() const override;
	size_t GetWindowWidth() const override;
	size_t GetWindowHeight() const  override;
	void SetCursorPosition(const glm::ivec2& CursorPosition) override;
	glm::ivec2 GetCursorPosition() const override;
	void ShowCursor() override;
	void HideCursor() override;
	void Close() override;
	void SetEventsListener(INativeWindowEventListener* WindowEventsListener) override;
	void ResetEventsListener() override;


	// IRenderWindow
	void SetRenderWindowEventListener(std::shared_ptr<IRenderWindowEventListener> RenderWindowEventListener) override;
	void SetNativeWindowEventListener(std::shared_ptr<INativeWindowEventListener> NativeWindowEventListener) override;
	const std::shared_ptr<IRenderTarget>& GetRenderTarget() const override;
	const Viewport& GetViewport() const override;


	// INativeWindowEventListener
	// Window events
	void OnWindowInputFocus(EventArgs& Args) override; // Window gets input focus
	void OnWindowInputBlur(EventArgs& Args) override;  // Window loses input focus
	void OnWindowMinimize(EventArgs& Args) override;   // Window is minimized.
	void OnWindowRestore(EventArgs& Args) override;    // Window is restored.
	void OnWindowResize(ResizeEventArgs& Args) override;
	void OnWindowClose(WindowCloseEventArgs& Args) override;
	// Keyboard events
	bool OnWindowKeyPressed(KeyEventArgs& Args) override;
	void OnWindowKeyReleased(KeyEventArgs& Args) override;
	void OnWindowKeyboardFocus(EventArgs& Args) override;
	void OnWindowKeyboardBlur(EventArgs& Args) override;
	// Mouse events
	void OnWindowMouseMoved(MouseMotionEventArgs& Args) override;
	bool OnWindowMouseButtonPressed(MouseButtonEventArgs& Args) override;
	void OnWindowMouseButtonReleased(MouseButtonEventArgs& Args) override;
	bool OnWindowMouseWheel(MouseWheelEventArgs& Args) override;
	void OnWindowMouseLeave(EventArgs& Args) override;
	void OnWindowMouseFocus(EventArgs& Args) override;
	void OnWindowMouseBlur(EventArgs& Args) override;


	// IApplicationEventsConnection
	void Connect(IApplicationEvents* ApplicationEvents) override;
	void Disconnect(IApplicationEvents* ApplicationEvents) override;

protected:
	virtual IRenderDevice&                          GetRenderDevice() const = 0;
    virtual void                                    CreateSwapChain();
    virtual void                                    ResizeSwapChainBuffers(uint32_t width, uint32_t height) = 0;

protected:
	INativeWindow&                                  m_NativeWindow;


	std::shared_ptr<IRenderWindowEventListener>     m_RenderWindowEventListener;
	std::shared_ptr<INativeWindowEventListener>     m_NativeWindowEventListener;
	std::shared_ptr<IRenderTarget>                  m_RenderTarget;
	Viewport                                        m_Viewport;
	
    bool                                            m_bResizePending;  


private: // IApplicationEventsConnection
	Delegate<UpdateEventArgs>::FunctionDecl         m_UpdateConnection;
};
