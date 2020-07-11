#include "stdafx.h"

// General
#include "ZenonWindowMinimal3D.h"

ZenonWindowMinimal3DWidget::ZenonWindowMinimal3DWidget(QWidget *parent)
	: QWidget(parent)

	, m_EventListener(nullptr)
{
	setAttribute(Qt::WA_NativeWindow);
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);
	//setAttribute(Qt::WA_NoMousePropagation);
}

ZenonWindowMinimal3DWidget::~ZenonWindowMinimal3DWidget()
{
}



//
// INativeWindow
//
void ZenonWindowMinimal3DWidget::SetWindowTitle(const std::string& WindowName)
{
}

std::string ZenonWindowMinimal3DWidget::GetWindowTitle() const
{
	return std::string();
}

size_t ZenonWindowMinimal3DWidget::GetWindowWidth() const
{
	return geometry().width();
}

size_t ZenonWindowMinimal3DWidget::GetWindowHeight() const
{
	return geometry().height();
}

void ZenonWindowMinimal3DWidget::SetCursorPosition(const glm::ivec2 & CursorPosition)
{
	_ASSERT(false);
}

glm::ivec2 ZenonWindowMinimal3DWidget::GetCursorPosition() const
{
	_ASSERT(false);
	return glm::ivec2();
}

void ZenonWindowMinimal3DWidget::ShowCursor()
{
	_ASSERT(false);
}

void ZenonWindowMinimal3DWidget::HideCursor()
{
	_ASSERT(false);
}

void ZenonWindowMinimal3DWidget::Close()
{
}

void ZenonWindowMinimal3DWidget::SetEventsListener(INativeWindowEventListener * WindowEventsListener)
{
	m_EventListener = WindowEventsListener;
}

void ZenonWindowMinimal3DWidget::ResetEventsListener()
{
	m_EventListener = nullptr;
}



//
// INativeWindow_WindowsSpecific
//
HWND ZenonWindowMinimal3DWidget::GetHWnd() const
{
	return (HWND)winId();
}

LRESULT ZenonWindowMinimal3DWidget::Windows_ProcessMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	_ASSERT(FALSE);
	return 0;
}


//
// Events
//

void ZenonWindowMinimal3DWidget::focusInEvent(QFocusEvent * event)
{
	m_EventListener->OnWindowInputFocus(EventArgs());
}

void ZenonWindowMinimal3DWidget::focusOutEvent(QFocusEvent * event)
{
	m_EventListener->OnWindowInputBlur(EventArgs());
}

void ZenonWindowMinimal3DWidget::enterEvent(QEvent * event)
{
	setFocus();

	m_EventListener->OnWindowMouseFocus(EventArgs());
}

void ZenonWindowMinimal3DWidget::leaveEvent(QEvent * event)
{
	m_EventListener->OnWindowMouseLeave(EventArgs());
}

void ZenonWindowMinimal3DWidget::paintEvent(QPaintEvent *pEvent)
{
	__super::paintEvent(pEvent);
}

void ZenonWindowMinimal3DWidget::moveEvent(QMoveEvent * event)
{
	// do nothing
}

void ZenonWindowMinimal3DWidget::resizeEvent(QResizeEvent * event)
{
	if (m_EventListener)
		m_EventListener->OnWindowResize(ResizeEventArgs(event->size().width(), event->size().height()));
}

void ZenonWindowMinimal3DWidget::closeEvent(QCloseEvent * event)
{
	
	m_EventListener->OnWindowClose(WindowCloseEventArgs());
}

void ZenonWindowMinimal3DWidget::showEvent(QShowEvent * event)
{
}

void ZenonWindowMinimal3DWidget::hideEvent(QHideEvent * event)
{
	//PostQuitMessage(0);
}
