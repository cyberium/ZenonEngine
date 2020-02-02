#pragma once

#include <QtWidgets/qframe.h>

class RenderWindowWidget
	: public QFrame
	, public INativeWindow
	, public INativeWindow_WindowsSpecific
	, public Object
{
	Q_OBJECT
public:
	RenderWindowWidget(QWidget * parent);
	virtual ~RenderWindowWidget();

	// INativeWindow
	void SetWindowTitle(const std::string& WindowName) override;
	std::string GetWindowTitle() const override;
	size_t GetWindowWidth() const override;
	size_t GetWindowHeight() const override;
	void SetCursorPosition(const glm::ivec2& CursorPosition);
	glm::ivec2 GetCursorPosition() const override;
	void ShowCursor();
	void HideCursor();
	void Close();
	void SetEventsListener(INativeWindowEventListener* WindowEventsListener);
	void ResetEventsListener();

	// INativeWindow_WindowsSpecific
	HWND GetHWnd() const override;
	LRESULT Windows_ProcessMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;


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
	void showEvent(QShowEvent* event) override;
	void hideEvent(QHideEvent* event) override;

private:
	INativeWindowEventListener* m_EventListener;
};
