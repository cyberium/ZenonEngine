#pragma once

#include <QtWidgets/QFrame>

class RenderWindowWidget
	: public QFrame
	, public INativeWindow
	, public Object
{
	Q_OBJECT
public:
	RenderWindowWidget(QWidget * parent);
	virtual ~RenderWindowWidget();



	//
	// INativeWindow
	//
	std::string GetWindowName();
	long GetWindowWidth();   
	long GetWindowHeight();   
	HWND GetHWnd();
	void SetCursorPosition(const glm::ivec2& CursorPosition);
	glm::ivec2 GetCursorPosition() const;
	void ShowCursor();
	void HideCursor();
	void SetEventsListener(INativeWindowEventListener* WindowEventsListener);
	void ResetEventsListener();
	LRESULT WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


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
	INativeWindowEventListener* m_EventListener;
};
