#pragma once

class RenderWindowWidget
	: public QFrame
	, public IWindowObject
	, public Object
{
	Q_OBJECT

public:
	RenderWindowWidget(QWidget * parent);
	virtual ~RenderWindowWidget();

	void SetRenderWindowEvents(std::shared_ptr<IRenderWindowEvents> RenderWindowEvents);

	QPaintEngine* paintEngine() const override { return nullptr; }

	// IWindowObject
	std::string GetWindowName();
	long GetWindowWidth();   
	long GetWindowHeight();   
	HWND GetHWnd();
	HDC BeginPaint(LPPAINTSTRUCT PaintStruct);
	BOOL EndPaint(LPPAINTSTRUCT PaintStruct);
	BOOL GetClientRect(LPRECT Rect);
	BOOL ClientToScreen(LPPOINT Point);
	BOOL ScreenToClient(LPPOINT Point);
	BOOL BringWindowToTop();
	BOOL ShowWindow(int nCmdShow);
	BOOL UpdateWindow();
	BOOL DestroyWindow();

private:
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
	std::shared_ptr<IRenderWindowEvents> m_RenderWindowEvents;
};
