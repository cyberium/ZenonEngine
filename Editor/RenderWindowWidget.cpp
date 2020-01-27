#include "stdafx.h"

// General
#include "RenderWindowWidget.h"


RenderWindowWidget::RenderWindowWidget(QWidget *parent)
	: QFrame(parent)
{
	setAttribute(Qt::WA_NativeWindow);
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);
}

RenderWindowWidget::~RenderWindowWidget()
{
}



//
// INativeWindow
//
void RenderWindowWidget::SetWindowTitle(std::string WindowName)
{
}

std::string RenderWindowWidget::GetWindowTitle() const
{
	return std::string();
}

long RenderWindowWidget::GetWindowWidth() const
{
	return geometry().width();
}

long RenderWindowWidget::GetWindowHeight() const
{
	return geometry().height();
}

void RenderWindowWidget::SetCursorPosition(const glm::ivec2 & CursorPosition)
{
	_ASSERT(false);
}

glm::ivec2 RenderWindowWidget::GetCursorPosition() const
{
	_ASSERT(false);
	return glm::ivec2();
}

void RenderWindowWidget::ShowCursor()
{
	_ASSERT(false);
}

void RenderWindowWidget::HideCursor()
{
	_ASSERT(false);
}

void RenderWindowWidget::SetEventsListener(INativeWindowEventListener * WindowEventsListener)
{
	m_EventListener = WindowEventsListener;
}

void RenderWindowWidget::ResetEventsListener()
{
	m_EventListener = nullptr;
}



//
// INativeWindow_WindowsSpecific
//
HWND RenderWindowWidget::GetHWnd() const
{
	return (HWND)winId();
}

LRESULT RenderWindowWidget::Windows_ProcessMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	_ASSERT(FALSE);
	return 0;
}



//
// Events
//
MouseButtonEventArgs::MouseButton QtToZenonMouseBotton(Qt::MouseButton qtState)
{
	switch (qtState)
	{
	case Qt::MouseButton::LeftButton:
		return MouseButtonEventArgs::MouseButton::Left;

	case Qt::MouseButton::RightButton:
		return MouseButtonEventArgs::MouseButton::Right;

	case Qt::MouseButton::MiddleButton:
		return MouseButtonEventArgs::MouseButton::Middel;
	}

	return MouseButtonEventArgs::MouseButton::None;
}

void RenderWindowWidget::mousePressEvent(QMouseEvent * event)
{
	MouseButtonEventArgs args
	(
		this,
		QtToZenonMouseBotton(event->button()),
		MouseButtonEventArgs::ButtonState::Pressed,
		event->buttons() == Qt::MouseButton::LeftButton,
		event->buttons() == Qt::MouseButton::MiddleButton,
		event->buttons() == Qt::MouseButton::RightButton,
		event->modifiers() == Qt::KeyboardModifier::ControlModifier,
		event->modifiers() == Qt::KeyboardModifier::ShiftModifier,
		event->x(),
		event->y()
	);
	m_EventListener->OnWindowMouseButtonPressed(args);
}

void RenderWindowWidget::mouseReleaseEvent(QMouseEvent * event)
{
	MouseButtonEventArgs args
	(
		this,
		QtToZenonMouseBotton(event->button()),
		MouseButtonEventArgs::ButtonState::Released,
		event->buttons() == Qt::MouseButton::LeftButton,
		event->buttons() == Qt::MouseButton::MiddleButton,
		event->buttons() == Qt::MouseButton::RightButton,
		event->modifiers() == Qt::KeyboardModifier::ControlModifier,
		event->modifiers() == Qt::KeyboardModifier::ShiftModifier,
		event->x(),
		event->y()
	);
	m_EventListener->OnWindowMouseButtonReleased(args);
}

void RenderWindowWidget::mouseDoubleClickEvent(QMouseEvent * event)
{
}

void RenderWindowWidget::mouseMoveEvent(QMouseEvent * event)
{
	MouseMotionEventArgs args
	(
		this,
		event->buttons() == Qt::MouseButton::LeftButton, 
		event->buttons() == Qt::MouseButton::MiddleButton, 
		event->buttons() == Qt::MouseButton::RightButton, 
		event->modifiers() == Qt::KeyboardModifier::ControlModifier, 
		event->modifiers() == Qt::KeyboardModifier::ShiftModifier, 
		event->x(), 
		event->y()
	);
	m_EventListener->OnWindowMouseMoved(args);
}

void RenderWindowWidget::wheelEvent(QWheelEvent * event)
{
	MouseWheelEventArgs args
	(
		this,
		event->delta(),
		event->buttons() == Qt::MouseButton::LeftButton,
		event->buttons() == Qt::MouseButton::MiddleButton,
		event->buttons() == Qt::MouseButton::RightButton,
		event->modifiers() == Qt::KeyboardModifier::ControlModifier,
		event->modifiers() == Qt::KeyboardModifier::ShiftModifier,
		event->x(),
		event->y()

	);
	m_EventListener->OnWindowMouseWheel(args);
}

void RenderWindowWidget::keyPressEvent(QKeyEvent * event)
{
	KeyEventArgs args
	(
		this,
		(KeyCode)event->nativeVirtualKey(),
		event->key(),
		KeyEventArgs::KeyState::Pressed,
		event->modifiers() == Qt::KeyboardModifier::ControlModifier,
		event->modifiers() == Qt::KeyboardModifier::ShiftModifier,
		event->modifiers() == Qt::KeyboardModifier::AltModifier
	);
	m_EventListener->OnWindowKeyPressed(args);
}

void RenderWindowWidget::keyReleaseEvent(QKeyEvent * event)
{
	KeyEventArgs args
	(
		this,
		(KeyCode)event->nativeVirtualKey(),
		event->key(),
		KeyEventArgs::KeyState::Released,
		event->modifiers() == Qt::KeyboardModifier::ControlModifier,
		event->modifiers() == Qt::KeyboardModifier::ShiftModifier,
		event->modifiers() == Qt::KeyboardModifier::AltModifier
	);
	m_EventListener->OnWindowKeyReleased(args);
}

void RenderWindowWidget::focusInEvent(QFocusEvent * event)
{
	m_EventListener->OnWindowInputFocus(EventArgs(this));
}

void RenderWindowWidget::focusOutEvent(QFocusEvent * event)
{
	m_EventListener->OnWindowInputBlur(EventArgs(this));
}

void RenderWindowWidget::enterEvent(QEvent * event)
{
	setFocus();

	m_EventListener->OnWindowMouseFocus(EventArgs(this));
}

void RenderWindowWidget::leaveEvent(QEvent * event)
{
	m_EventListener->OnWindowMouseLeave(EventArgs(this));
}

void RenderWindowWidget::paintEvent(QPaintEvent *pEvent)
{
	// do nothing
}

void RenderWindowWidget::moveEvent(QMoveEvent * event)
{
	// do nothing
}

void RenderWindowWidget::resizeEvent(QResizeEvent * event)
{
	m_EventListener->OnWindowResize(ResizeEventArgs(this, event->size().width(), event->size().height()));
}

void RenderWindowWidget::closeEvent(QCloseEvent * event)
{
	m_EventListener->OnWindowClose(WindowCloseEventArgs(this));
}
