#pragma once

class RenderWindowWidget
	: public QFrame
	, public IWindowObject
{
	Q_OBJECT

public:
	RenderWindowWidget(QWidget * parent);
	virtual ~RenderWindowWidget();

	void SetRenderWindow(std::shared_ptr<RenderWindow> RenderWindow);

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
	QAbstractEventDispatcher *dispatcher;
	QTime lastAwake;
	QTime lastBlock;

	HWND m_hwnd;

	std::shared_ptr<RenderWindow> m_RenderWindow;
};
