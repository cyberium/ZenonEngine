#include "stdafx.h"

// General
#include "RenderWindowWidget.h"


RenderWindowWidget::RenderWindowWidget(QWidget *parent)
	: QFrame(parent)
{
	setAttribute(Qt::WA_NativeWindow);
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);

	m_hwnd = (HWND)winId();
}

RenderWindowWidget::~RenderWindowWidget()
{
}

void RenderWindowWidget::SetRenderWindow(std::shared_ptr<RenderWindow> RenderWindow)
{
	m_RenderWindow = RenderWindow;
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
	return m_hwnd;
}

HDC RenderWindowWidget::BeginPaint(LPPAINTSTRUCT PaintStruct)
{
	return HDC();
}

BOOL RenderWindowWidget::EndPaint(LPPAINTSTRUCT PaintStruct)
{
	return 0;
}

BOOL RenderWindowWidget::GetClientRect(LPRECT Rect)
{
	Rect->left = geometry().x();
	Rect->right = geometry().x() + geometry().width();

	Rect->top = geometry().y();
	Rect->bottom = geometry().y() + geometry().height();

	return TRUE;
}

BOOL RenderWindowWidget::ClientToScreen(LPPOINT Point)
{
	return 0;
}

BOOL RenderWindowWidget::ScreenToClient(LPPOINT Point)
{
	return 0;
}

BOOL RenderWindowWidget::BringWindowToTop()
{
	return 0;
}

BOOL RenderWindowWidget::ShowWindow(int nCmdShow)
{
	return 0;
}

BOOL RenderWindowWidget::UpdateWindow()
{
	return 0;
}

BOOL RenderWindowWidget::DestroyWindow()
{
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
		*m_RenderWindow,
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
	m_RenderWindow->OnMouseButtonPressed(args);
}

void RenderWindowWidget::mouseReleaseEvent(QMouseEvent * event)
{
	MouseButtonEventArgs args
	(
		*m_RenderWindow,
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
	m_RenderWindow->OnMouseButtonReleased(args);
}

void RenderWindowWidget::mouseDoubleClickEvent(QMouseEvent * event)
{
}

void RenderWindowWidget::mouseMoveEvent(QMouseEvent * event)
{
	MouseMotionEventArgs args
	(
		*m_RenderWindow, 
		event->buttons() == Qt::MouseButton::LeftButton, 
		event->buttons() == Qt::MouseButton::MiddleButton, 
		event->buttons() == Qt::MouseButton::RightButton, 
		event->modifiers() == Qt::KeyboardModifier::ControlModifier, 
		event->modifiers() == Qt::KeyboardModifier::ShiftModifier, 
		event->x(), 
		event->y()
	);
	m_RenderWindow->OnMouseMoved(args);
}

void RenderWindowWidget::wheelEvent(QWheelEvent * event)
{
	MouseWheelEventArgs args
	(
		*m_RenderWindow,
		event->delta(),
		event->buttons() == Qt::MouseButton::LeftButton,
		event->buttons() == Qt::MouseButton::MiddleButton,
		event->buttons() == Qt::MouseButton::RightButton,
		event->modifiers() == Qt::KeyboardModifier::ControlModifier,
		event->modifiers() == Qt::KeyboardModifier::ShiftModifier,
		event->x(),
		event->y()

	);
	m_RenderWindow->OnMouseWheel(args);
}

void RenderWindowWidget::keyPressEvent(QKeyEvent * event)
{
	KeyEventArgs args
	(
		*m_RenderWindow,
		(KeyCode)event->key(),
		event->key(),
		KeyEventArgs::KeyState::Pressed,
		event->modifiers() == Qt::KeyboardModifier::ControlModifier,
		event->modifiers() == Qt::KeyboardModifier::ShiftModifier,
		event->modifiers() == Qt::KeyboardModifier::AltModifier
	);
	m_RenderWindow->OnKeyPressed(args);
}

void RenderWindowWidget::keyReleaseEvent(QKeyEvent * event)
{
	KeyEventArgs args
	(
		*m_RenderWindow,
		(KeyCode)event->key(),
		event->key(),
		KeyEventArgs::KeyState::Released,
		event->modifiers() == Qt::KeyboardModifier::ControlModifier,
		event->modifiers() == Qt::KeyboardModifier::ShiftModifier,
		event->modifiers() == Qt::KeyboardModifier::AltModifier
	);
	m_RenderWindow->OnKeyReleased(args);
}

void RenderWindowWidget::focusInEvent(QFocusEvent * event)
{
}

void RenderWindowWidget::focusOutEvent(QFocusEvent * event)
{
}

void RenderWindowWidget::enterEvent(QEvent * event)
{
	setFocus();
}

void RenderWindowWidget::leaveEvent(QEvent * event)
{
}

void RenderWindowWidget::paintEvent(QPaintEvent *pEvent)
{
}

void RenderWindowWidget::moveEvent(QMoveEvent * event)
{
}

void RenderWindowWidget::resizeEvent(QResizeEvent * event)
{
	ResizeEventArgs args
	(
		*m_RenderWindow,
		event->size().width(),
		event->size().height()
	);
	m_RenderWindow->OnResize(args);
}

void RenderWindowWidget::closeEvent(QCloseEvent * event)
{
}

/*bool Direct3DWidget::nativeEvent(const QByteArray & eventType, void * message, long * result)
{
	MSG * uMsg = static_cast<MSG*>(message);

	return Application::WndProc(uMsg->hwnd, uMsg->message, uMsg->wParam, uMsg->lParam);
}*/
