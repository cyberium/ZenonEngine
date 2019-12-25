#pragma once

class RenderWindowWidget
	: public QFrame
	, public IWindowObject
	, public IWindowEvents
	, public Object
{
	Q_OBJECT
public:
	RenderWindowWidget(QWidget * parent);
	virtual ~RenderWindowWidget();



	//
	// IWindowObject
	//
	std::string GetWindowName();
	long GetWindowWidth();   
	long GetWindowHeight();   
	HWND GetHWnd();
	void SetCursorPosition(const glm::ivec2& CursorPosition);
	glm::ivec2 GetCursorPosition() const;
	void ShowCursor();
	void HideCursor();



	//
	// IWindowEvents
	//

	// Window events
	Event&				InputFocus(); // Window gets input focus
	Event&				InputBlur();  // Window loses input focus
	Event&				Minimize();   // Window is minimized.
	Event&				Restore();    // Window is restored.
	ResizeEvent&        Resize();

	// Window is closing
	WindowCloseEvent&   Close();

	// Keyboard events
	KeyboardEvent&      KeyPressed();
	KeyboardEvent&      KeyReleased();
	Event&              KeyboardFocus();
	Event&              KeyboardBlur();

	// Mouse events
	MouseMotionEvent&   MouseMoved();
	MouseButtonEvent&   MouseButtonPressed();
	MouseButtonEvent&   MouseButtonReleased();
	MouseWheelEvent&    MouseWheel();
	Event&              MouseLeave();
	Event&              MouseFocus();
	Event&              MouseBlur();


private:
	QPaintEngine* paintEngine() const override { return nullptr; } // don't touch this magic!!!

	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseDoubleClickEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;
	void keyReleaseEvent(QKeyEvent *event) override;
	void focusInEvent(QFocusEvent *event) override;
	void focusOutEvent(QFocusEvent *event) override;
	void enterEvent(QEvent *event) override;
	void leaveEvent(QEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
	void moveEvent(QMoveEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;
	void closeEvent(QCloseEvent *event) override;


private:
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
};
