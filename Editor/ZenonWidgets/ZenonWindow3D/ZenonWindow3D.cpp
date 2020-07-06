#include "stdafx.h"

// General
#include "ZenonWindow3D.h"

ZenonWindow3D::ZenonWindow3D(QWidget *parent)
	: ZenonWindowMinimal3DWidget(parent)
	, m_Editor3D(nullptr)
	, m_EditorUI(nullptr)
{
	this->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));
}

ZenonWindow3D::~ZenonWindow3D()
{}

//
// Slots
//
void ZenonWindow3D::onCustomContextMenu(const QPoint& pos)
{
	QMenu * menu = new QMenu(this);

	auto node = m_Editor3D->GetNodeUnderMouse(glm::ivec2(pos.x(), pos.y()));
	if (node == nullptr)
		return;

	if (! m_EditorUI->ExtendContextMenu(menu, node))
		return;

	menu->popup(mapToGlobal(pos));
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

void ZenonWindow3D::dropEvent(QDropEvent * event)
{
	//event->acceptProposedAction();
	m_Editor3D->DropEvent(glm::vec2(event->pos().x(), event->pos().y()));
}

void ZenonWindow3D::dragEnterEvent(QDragEnterEvent * event)
{
	const QMimeData *mimeData = event->mimeData();
	if (mimeData->hasText()) 
	{
		std::string text = mimeData->text().toStdString();

		SDragData data;
		data.Message = text;
		data.Position = glm::vec2(event->pos().x(), event->pos().y());
		data.IsCtrl = (event->keyboardModifiers() & Qt::KeyboardModifier::ControlModifier) != 0;
		try
		{
			m_Editor3D->DragEnterEvent(data);
		}
		catch (...)
		{
			Log::Error("Error!");
		}

	}
	else 
	{
	//	_ASSERT(false);
	}

	event->setDropAction(Qt::DropAction::MoveAction);
	event->acceptProposedAction();
}

void ZenonWindow3D::dragMoveEvent(QDragMoveEvent * event)
{
	//repaint();
	m_Editor3D->DragMoveEvent(glm::vec2(event->pos().x(), event->pos().y()));
}

void ZenonWindow3D::dragLeaveEvent(QDragLeaveEvent * event)
{
	m_Editor3D->DragLeaveEvent();
}
