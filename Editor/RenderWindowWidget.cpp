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

std::string RenderWindowWidget::GetWindowName()
{
	return std::string();
}

long RenderWindowWidget::GetWindowWidth()
{
	return geometry().width();
}

long RenderWindowWidget::GetWindowHeight()
{
	return geometry().height();
}

HWND RenderWindowWidget::GetHWnd()
{
	return (HWND)winId();
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



//
// IWindowEvents
//

// Window events
Event& RenderWindowWidget::InputFocus()
{
	return m_InputFocus;
}

Event& RenderWindowWidget::InputBlur()
{
	return m_InputBlur;
}

Event& RenderWindowWidget::Minimize()
{
	return m_Minimize;
}

Event& RenderWindowWidget::Restore()
{
	return m_Restore;
}

ResizeEvent& RenderWindowWidget::Resize()
{
	return m_Resize;
}

WindowCloseEvent& RenderWindowWidget::Close()
{
	return m_Close;
}



// Keyboard events
KeyboardEvent& RenderWindowWidget::KeyPressed()
{
	return m_KeyPressed;
}

KeyboardEvent& RenderWindowWidget::KeyReleased()
{
	return m_KeyReleased;
}

Event& RenderWindowWidget::KeyboardFocus()
{
	return m_KeyboardFocus;
}

Event& RenderWindowWidget::KeyboardBlur()
{
	return m_KeyboardBlur;
}


// Mouse events
MouseMotionEvent& RenderWindowWidget::MouseMoved()
{
	return m_MouseMoved;
}

MouseButtonEvent& RenderWindowWidget::MouseButtonPressed()
{
	return m_MouseButtonPressed;
}

MouseButtonEvent& RenderWindowWidget::MouseButtonReleased()
{
	return m_MouseButtonReleased;
}

MouseWheelEvent& RenderWindowWidget::MouseWheel()
{
	return m_MouseWheel;
}

Event& RenderWindowWidget::MouseLeave()
{
	return m_MouseLeave;
}

Event& RenderWindowWidget::MouseFocus()
{
	return m_MouseFocus;
}

Event& RenderWindowWidget::MouseBlur()
{
	return m_MouseBlur;
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
	m_MouseButtonPressed(args);
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
	m_MouseButtonReleased(args);
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
	m_MouseMoved(args);
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
	m_MouseWheel(args);
}

void RenderWindowWidget::keyPressEvent(QKeyEvent * event)
{
	KeyEventArgs args
	(
		this,
		(KeyCode)event->key(),
		event->key(),
		KeyEventArgs::KeyState::Pressed,
		event->modifiers() == Qt::KeyboardModifier::ControlModifier,
		event->modifiers() == Qt::KeyboardModifier::ShiftModifier,
		event->modifiers() == Qt::KeyboardModifier::AltModifier
	);
	m_KeyPressed(args);
}

void RenderWindowWidget::keyReleaseEvent(QKeyEvent * event)
{
	KeyEventArgs args
	(
		this,
		(KeyCode)event->key(),
		event->key(),
		KeyEventArgs::KeyState::Released,
		event->modifiers() == Qt::KeyboardModifier::ControlModifier,
		event->modifiers() == Qt::KeyboardModifier::ShiftModifier,
		event->modifiers() == Qt::KeyboardModifier::AltModifier
	);
	m_KeyReleased(args);
}

void RenderWindowWidget::focusInEvent(QFocusEvent * event)
{
	m_InputFocus(EventArgs(this));
}

void RenderWindowWidget::focusOutEvent(QFocusEvent * event)
{
	m_InputBlur(EventArgs(this));
}

void RenderWindowWidget::enterEvent(QEvent * event)
{
	setFocus();

	m_MouseFocus(EventArgs(this));
}

void RenderWindowWidget::leaveEvent(QEvent * event)
{
	m_MouseLeave(EventArgs(this));
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
	m_Resize(ResizeEventArgs(this, event->size().width(), event->size().height()));
}

void RenderWindowWidget::closeEvent(QCloseEvent * event)
{
	m_Close(WindowCloseEventArgs(this));
}
