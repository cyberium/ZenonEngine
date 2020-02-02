#pragma once

class ZN_API RenderWindowBase 
	: public IRenderWindow
	, public IRenderWindowEvents
	, public INativeWindowEventListener
	, public IApplicationEventsConnection
	, public Object
	, public std::enable_shared_from_this<IRenderWindow>
{
public:
	RenderWindowBase(INativeWindow& WindowObject, bool vSync = false);
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
	bool                                            IsVSync() const override;
	virtual void                                    Present() = 0;
	const std::shared_ptr<IRenderTarget>&           GetRenderTarget() const override;
	const Viewport&									GetViewport() const override;


	//
	// IRenderWindowEvents
	//
	UpdateEvent&        Update() override;
	RenderEvent&        PreRender() override;
	RenderEvent&        Render() override;
	RenderEvent&        PostRender() override;
	RenderEvent&        RenderUI() override;

	// Window events
	Event&				WindowInputFocus() override; // Window gets input focus
	Event&				WindowInputBlur() override;  // Window loses input focus
	Event&				WindowMinimize() override;   // Window is minimized.
	Event&				WindowRestore() override;    // Window is restored.
	ResizeEvent&        WindowResize() override;
	WindowCloseEvent&   WindowClose() override;

	// Keyboard events
	KeyboardEvent&      WindowKeyPressed() override;
	KeyboardEvent&      WindowKeyReleased() override;
	Event&              WindowKeyboardFocus() override;
	Event&              WindowKeyboardBlur() override;

	// Mouse events
	MouseMotionEvent&   WindowMouseMoved() override;
	MouseButtonEvent&   WindowMouseButtonPressed() override;
	MouseButtonEvent&   WindowMouseButtonReleased() override;
	MouseWheelEvent&    WindowMouseWheel() override;
	Event&              WindowMouseLeave() override;
	Event&              WindowMouseFocus() override;
	Event&              WindowMouseBlur() override;


	//
	// INativeWindowEventListener
	//
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
	void											Connect(IApplicationEvents* ApplicationEvents) override;
	void											Disconnect(IApplicationEvents* ApplicationEvents) override;

protected:
	virtual IRenderDevice&                          GetRenderDevice() const = 0;
    virtual void                                    CreateSwapChain();
    virtual void                                    ResizeSwapChainBuffers(uint32_t width, uint32_t height) = 0;

protected:
	INativeWindow&                                  m_NativeWindow;
	bool                                            m_vSync;
	std::shared_ptr<IRenderTarget>                  m_RenderTarget;
	Viewport                                        m_Viewport;
	
    bool                                            m_bResizePending;  


private: // IApplicationEventsConnection
	Delegate<UpdateEventArgs>::FunctionDecl         m_UpdateConnection;

private:
	// IRenderWindowEvents
	UpdateEvent			m_Update;
	RenderEvent         m_PreRender;
	RenderEvent         m_Render;
	RenderEvent         m_PostRender;
	RenderEvent         m_RenderUI;

	// Window events
	Event				m_InputFocus; // Window gets input focus
	Event				m_InputBlur;  // Window loses input focus
	Event				m_Minimize;   // Window is minimized.
	Event				m_Restore;    // Window is restored.
	ResizeEvent         m_Resize;

	// Window is closing
	WindowCloseEvent    m_Close;

	// Keyboard events
	KeyboardEvent       m_KeyPressed;
	KeyboardEvent       m_KeyReleased;
	Event               m_KeyboardFocus;
	Event               m_KeyboardBlur;

	// Mouse events
	MouseMotionEvent    m_MouseMoved;
	MouseButtonEvent    m_MouseButtonPressed;
	MouseButtonEvent    m_MouseButtonReleased;
	MouseWheelEvent     m_MouseWheel;
	Event               m_MouseLeave;
	Event               m_MouseFocus;
	Event               m_MouseBlur;
};
