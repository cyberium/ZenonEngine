#pragma once

class OW_ENGINE_API RenderWindowBase 
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
	void                                            ShowWindow(); // Show this window if it is hidden.
	void                                            HideWindow(); // Hide the window. The window will not be destroyed and can be  shown again using the ShowWindow() function.
	void                                            CloseWindow(); // Destroy and close the window.

	void                                            SetMousePosition(vec2 _position);

	int                                             GetWindowWidth() const;
	int                                             GetWindowHeight() const;
	glm::ivec2                                      GetWindowSize() const;
	bool                                            IsVSync() const;
	HWND                                            GetHWnd() const;

	virtual void                                    Present() = 0;

    std::shared_ptr<IRenderDevice>                  GetRenderDevice() const;
	std::shared_ptr<IRenderTarget>                  GetRenderTarget() const;
	const Viewport *                                GetViewport() const;
	LRESULT											WndProc(HWND, UINT, WPARAM, LPARAM);

	// IRenderWindowEvents
	Event&				Initialize();
	virtual      void OnInitialize(EventArgs& e);
	UpdateEvent&		Update();
	virtual      void OnUpdate(UpdateEventArgs& e);
	RenderEvent&        PreRender();
	virtual      void OnPreRender(RenderEventArgs& e);
	RenderEvent&        Render();
	virtual      void OnRender(RenderEventArgs& e);
	RenderEvent&        PostRender();
	virtual      void OnPostRender(RenderEventArgs& e);
	RenderEvent&        RenderUI();
	virtual      void OnRenderUI(RenderEventArgs& e);
	Event&				Terminate();
	virtual      void OnTerminate(EventArgs& e);
	// Window events
	Event&				InputFocus(); // Window gets input focus
	virtual      void OnInputFocus(EventArgs& e);
	Event&				InputBlur();  // Window loses input focus
	virtual      void OnInputBlur(EventArgs& e);
	Event&				Minimize();   // Window is minimized.
	virtual      void OnMinimize(EventArgs& e);
	Event&				Restore();    // Window is restored.
	virtual      void OnRestore(EventArgs& e);
	ResizeEvent&        Resize();
	virtual      void OnResize(ResizeEventArgs& e);
	Event&				Expose();
	virtual      void OnExpose(EventArgs& e);
	// Window is closing
	WindowCloseEvent&   Close();
	virtual      void OnClose(WindowCloseEventArgs& e);
	// Keyboard events
	KeyboardEvent&       KeyPressed();
	virtual       void OnKeyPressed(KeyEventArgs& e);
	KeyboardEvent&       KeyReleased();
	virtual      void OnKeyReleased(KeyEventArgs& e);
	Event&               KeyboardFocus();
	virtual      void OnKeyboardFocus(EventArgs& e);
	Event&               KeyboardBlur();
	virtual      void OnKeyboardBlur(EventArgs& e);
	// Mouse events
	MouseMotionEvent&   MouseMoved();
	virtual      void OnMouseMoved(MouseMotionEventArgs& e);
	MouseButtonEvent&   MouseButtonPressed();
	virtual      void OnMouseButtonPressed(MouseButtonEventArgs& e);
	MouseButtonEvent&   MouseButtonReleased();
	virtual      void OnMouseButtonReleased(MouseButtonEventArgs& e);
	MouseWheelEvent&    MouseWheel();
	virtual      void OnMouseWheel(MouseWheelEventArgs& e);
	Event&              MouseLeave();
	virtual      void OnMouseLeave(EventArgs& e);
	Event&              MouseFocus();
	virtual      void OnMouseFocus(EventArgs& e);
	Event&              MouseBlur();
	virtual      void OnMouseBlur(EventArgs& e);

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

	// For mouse events
	glm::ivec2                                      m_PreviousMousePosition;  // Used to compute relative mouse motion in this window.
	bool                                            m_InClientRect;                // This is true when the mouse is inside the window's client rect.
	bool                                            m_IsMouseTracking;             // Used to capture mouse enter/leave events.

	// For keyboard events
	bool                                            m_bHasKeyboardFocus;            // This is set to true when the window receives keyboard focus.

private:
	// IRenderWindowEvents
	Event				m_Initialize;
	UpdateEvent			m_Update;
	RenderEvent         m_PreRender;
	RenderEvent         m_Render;
	RenderEvent         m_PostRender;
	RenderEvent         m_RenderUI;
	Event				m_Terminate;

	// Window events
	Event				m_InputFocus; // Window gets input focus
	Event				m_InputBlur;  // Window loses input focus
	Event				m_Minimize;   // Window is minimized.
	Event				m_Restore;    // Window is restored.
	ResizeEvent         m_Resize;
	Event				m_Expose;

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

private: // IApplicationEventsConnection
	Delegate<EventArgs>::FunctionDecl        m_InitializeConnection;
	Delegate<UpdateEventArgs>::FunctionDecl  m_UpdateConnection;
	Delegate<EventArgs>::FunctionDecl        m_TerminateConnection;
	//Delegate<EventArgs>::FunctionDecl        m_TerminatedConnection;
	//Delegate<EventArgs>::FunctionDecl		 m_ExitConnection;
	//Delegate<UserEventArgs>::FunctionDecl	 m_UserEventConnection;
};
