#include "stdafx.h"

// General
#include "ZenonWindow3D.h"

ZenonWindow3D::ZenonWindow3D(QWidget *parent)
	: QWidget(parent)
	, m_Editor3D(nullptr)
	, m_EditorUI(nullptr)
{
	//setAttribute(Qt::WA_NativeWindow);
	//setAttribute(Qt::WA_PaintOnScreen);
	//setAttribute(Qt::WA_NoSystemBackground);
	//setAttribute(Qt::WA_NoMousePropagation);

	this->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(slotCustomMenuRequested(const QPoint &)));
}

ZenonWindow3D::~ZenonWindow3D()
{
}



//
// INativeWindow
//
void ZenonWindow3D::SetWindowTitle(const std::string& WindowName)
{
}

std::string ZenonWindow3D::GetWindowTitle() const
{
	return std::string();
}

size_t ZenonWindow3D::GetWindowWidth() const
{
	return geometry().width();
}

size_t ZenonWindow3D::GetWindowHeight() const
{
	return geometry().height();
}

void ZenonWindow3D::SetCursorPosition(const glm::ivec2 & CursorPosition)
{
	_ASSERT(false);
}

glm::ivec2 ZenonWindow3D::GetCursorPosition() const
{
	_ASSERT(false);
	return glm::ivec2();
}

void ZenonWindow3D::ShowCursor()
{
	_ASSERT(false);
}

void ZenonWindow3D::HideCursor()
{
	_ASSERT(false);
}

void ZenonWindow3D::Close()
{
}

void ZenonWindow3D::SetEventsListener(INativeWindowEventListener * WindowEventsListener)
{
	m_EventListener = WindowEventsListener;
}

void ZenonWindow3D::ResetEventsListener()
{
	m_EventListener = nullptr;
}



//
// INativeWindow_WindowsSpecific
//
HWND ZenonWindow3D::GetHWnd() const
{
	return (HWND)winId();
}

LRESULT ZenonWindow3D::Windows_ProcessMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	_ASSERT(FALSE);
	return 0;
}



//
// Slots
//
void ZenonWindow3D::slotCustomMenuRequested(const QPoint& pos)
{
	QMenu * menu = new QMenu(this);

	auto node = m_Editor3D->GetNodeUnderMouse(glm::ivec2(pos.x(), pos.y()));
	if (node == nullptr)
		return;

	m_EditorUI->ExtendContextMenu(menu, node);

	menu->popup(mapToGlobal(pos));
}

void ZenonWindow3D::slotEditRecord()
{
}

void ZenonWindow3D::slotRemoveRecord()
{
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

void ZenonWindow3D::mousePressEvent(QMouseEvent * event)
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

void ZenonWindow3D::mouseReleaseEvent(QMouseEvent * event)
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

void ZenonWindow3D::mouseDoubleClickEvent(QMouseEvent * event)
{
}

void ZenonWindow3D::mouseMoveEvent(QMouseEvent * event)
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

void ZenonWindow3D::wheelEvent(QWheelEvent * event)
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

void ZenonWindow3D::keyPressEvent(QKeyEvent * event)
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

void ZenonWindow3D::keyReleaseEvent(QKeyEvent * event)
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

void ZenonWindow3D::focusInEvent(QFocusEvent * event)
{
	m_EventListener->OnWindowInputFocus(EventArgs(this));
}

void ZenonWindow3D::focusOutEvent(QFocusEvent * event)
{
	m_EventListener->OnWindowInputBlur(EventArgs(this));
}

void ZenonWindow3D::enterEvent(QEvent * event)
{
	setFocus();

	m_EventListener->OnWindowMouseFocus(EventArgs(this));
}

void ZenonWindow3D::leaveEvent(QEvent * event)
{
	m_EventListener->OnWindowMouseLeave(EventArgs(this));
}

void ZenonWindow3D::paintEvent(QPaintEvent *pEvent)
{
	__super::paintEvent(pEvent);
}

void ZenonWindow3D::moveEvent(QMoveEvent * event)
{
	// do nothing
}

void ZenonWindow3D::resizeEvent(QResizeEvent * event)
{
	m_EventListener->OnWindowResize(ResizeEventArgs(this, event->size().width(), event->size().height()));
}

void ZenonWindow3D::closeEvent(QCloseEvent * event)
{
	
	m_EventListener->OnWindowClose(WindowCloseEventArgs(this));
}

void ZenonWindow3D::showEvent(QShowEvent * event)
{
}

void ZenonWindow3D::hideEvent(QHideEvent * event)
{
	PostQuitMessage(0);
}
