#include "stdafx.h"

// General
#include "RenderWindowWidget.h"


Direct3DWidget::Direct3DWidget(QWidget *parent)
	: QFrame(parent)
{
	setAttribute(Qt::WA_NativeWindow);
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);

	m_hwnd = (HWND)winId();
}

Direct3DWidget::~Direct3DWidget()
{
}

void Direct3DWidget::SetRenderWindow(std::shared_ptr<RenderWindow> RenderWindow)
{
	m_RenderWindow = RenderWindow;
}

std::string Direct3DWidget::GetWindowName()
{
	return std::string();
}

long Direct3DWidget::GetWindowWidth()
{
	return geometry().width();
}

long Direct3DWidget::GetWindowHeight()
{
	return geometry().height();
}

HWND Direct3DWidget::GetHWnd()
{
	return m_hwnd;
}

HDC Direct3DWidget::BeginPaint(LPPAINTSTRUCT PaintStruct)
{
	return HDC();
}

BOOL Direct3DWidget::EndPaint(LPPAINTSTRUCT PaintStruct)
{
	return 0;
}

BOOL Direct3DWidget::GetClientRect(LPRECT Rect)
{
	Rect->left = geometry().x();
	Rect->right = geometry().x() + geometry().width();

	Rect->top = geometry().y();
	Rect->bottom = geometry().y() + geometry().height();

	return TRUE;
}

BOOL Direct3DWidget::ClientToScreen(LPPOINT Point)
{
	return 0;
}

BOOL Direct3DWidget::ScreenToClient(LPPOINT Point)
{
	return 0;
}

BOOL Direct3DWidget::BringWindowToTop()
{
	return 0;
}

BOOL Direct3DWidget::ShowWindow(int nCmdShow)
{
	return 0;
}

BOOL Direct3DWidget::UpdateWindow()
{
	return 0;
}

BOOL Direct3DWidget::DestroyWindow()
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

void Direct3DWidget::mousePressEvent(QMouseEvent * event)
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

void Direct3DWidget::mouseReleaseEvent(QMouseEvent * event)
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

void Direct3DWidget::mouseDoubleClickEvent(QMouseEvent * event)
{
}

void Direct3DWidget::mouseMoveEvent(QMouseEvent * event)
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

void Direct3DWidget::wheelEvent(QWheelEvent * event)
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

void Direct3DWidget::keyPressEvent(QKeyEvent * event)
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

void Direct3DWidget::keyReleaseEvent(QKeyEvent * event)
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

void Direct3DWidget::focusInEvent(QFocusEvent * event)
{
}

void Direct3DWidget::focusOutEvent(QFocusEvent * event)
{
}

void Direct3DWidget::enterEvent(QEvent * event)
{
	setFocus();
}

void Direct3DWidget::leaveEvent(QEvent * event)
{
}

void Direct3DWidget::paintEvent(QPaintEvent *pEvent)
{
}

void Direct3DWidget::moveEvent(QMoveEvent * event)
{
}

void Direct3DWidget::resizeEvent(QResizeEvent * event)
{
	ResizeEventArgs args
	(
		*m_RenderWindow,
		event->size().width(),
		event->size().height()
	);
	m_RenderWindow->OnResize(args);
}

void Direct3DWidget::closeEvent(QCloseEvent * event)
{
}

/*bool Direct3DWidget::nativeEvent(const QByteArray & eventType, void * message, long * result)
{
	MSG * uMsg = static_cast<MSG*>(message);

	return Application::WndProc(uMsg->hwnd, uMsg->message, uMsg->wParam, uMsg->lParam);
}*/
