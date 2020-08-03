#pragma once

#include <qframe.h>

class ZenonWindowMinimal3DWidget
	: public QWidget
	, public INativeWindow
	, public INativeWindow_WindowsSpecific
{
	Q_OBJECT
public:
	ZenonWindowMinimal3DWidget(QWidget * parent);
	virtual ~ZenonWindowMinimal3DWidget();

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

protected:
	QPaintEngine* paintEngine() const override { return nullptr; } // don't touch this magic!!!

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

protected:
	glm::ivec2 m_PreviousMousePosition;
	INativeWindowEventListener* m_EventListener;
};
