#include "stdafx.h"

// General
#include "ZenonWindow3D.h"

ZenonWindow3D::ZenonWindow3D(QWidget *parent)
	: ZenonWindowMinimal3DWidget(parent)
{

}

ZenonWindow3D::~ZenonWindow3D()
{}




//
// Events
//
namespace
{
	MouseButton QtToZenonMouseBotton(Qt::MouseButton qtState)
	{
		switch (qtState)
		{
		case Qt::MouseButton::LeftButton:
			return MouseButton::Left;

		case Qt::MouseButton::RightButton:
			return MouseButton::Right;

		case Qt::MouseButton::MiddleButton:
			return MouseButton::Middel;
		}

		return MouseButton::None;
	}
}

void ZenonWindow3D::mousePressEvent(QMouseEvent * event)
{
	MouseButtonEventArgs args
	(
		QtToZenonMouseBotton(event->button()),
		ButtonState::Pressed,
		event->buttons() == Qt::MouseButton::LeftButton,
		event->buttons() == Qt::MouseButton::MiddleButton,
		event->buttons() == Qt::MouseButton::RightButton,
		event->modifiers() == Qt::KeyboardModifier::ControlModifier,
		event->modifiers() == Qt::KeyboardModifier::ShiftModifier,
		event->modifiers() == Qt::KeyboardModifier::AltModifier,
		event->x(),
		event->y()
	);
	m_EventListener->OnWindowMouseButtonPressed(args);
}

void ZenonWindow3D::mouseReleaseEvent(QMouseEvent * event)
{
	MouseButtonEventArgs args
	(
		QtToZenonMouseBotton(event->button()),
		ButtonState::Released,
		event->buttons() == Qt::MouseButton::LeftButton,
		event->buttons() == Qt::MouseButton::MiddleButton,
		event->buttons() == Qt::MouseButton::RightButton,
		event->modifiers() == Qt::KeyboardModifier::ControlModifier,
		event->modifiers() == Qt::KeyboardModifier::ShiftModifier,
		event->modifiers() == Qt::KeyboardModifier::AltModifier,
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
		event->buttons() == Qt::MouseButton::LeftButton, 
		event->buttons() == Qt::MouseButton::MiddleButton, 
		event->buttons() == Qt::MouseButton::RightButton, 
		event->modifiers() == Qt::KeyboardModifier::ControlModifier, 
		event->modifiers() == Qt::KeyboardModifier::ShiftModifier, 
		event->x(), 
		event->y()
	);
	args.RelX = args.X - m_PreviousMousePosition.x;
	args.RelY = args.Y - m_PreviousMousePosition.y;
	m_PreviousMousePosition = glm::ivec2(args.X, args.Y);
	m_EventListener->OnWindowMouseMoved(args);
}

void ZenonWindow3D::wheelEvent(QWheelEvent * event)
{
	MouseWheelEventArgs args
	(
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
		(KeyCode)event->nativeVirtualKey(),
		event->key(),
		KeyState::Pressed,
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
		(KeyCode)event->nativeVirtualKey(),
		event->key(),
		KeyState::Released,
		event->modifiers() == Qt::KeyboardModifier::ControlModifier,
		event->modifiers() == Qt::KeyboardModifier::ShiftModifier,
		event->modifiers() == Qt::KeyboardModifier::AltModifier
	);
	m_EventListener->OnWindowKeyReleased(args);
}
