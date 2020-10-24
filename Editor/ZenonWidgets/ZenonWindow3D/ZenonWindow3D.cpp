#include "stdafx.h"

// General
#include "ZenonWindow3D.h"

ZenonWindow3D::ZenonWindow3D(QWidget *parent)
	: ZenonWindowMinimal3DWidget(parent)
	, m_Editor(nullptr)
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
	QMenu * menu = DEBUG_NEW QMenu(this);

	auto node = m_Editor->Get3DFrame().GetNodeUnderMouse(glm::ivec2(pos.x(), pos.y()));
	if (node == nullptr)
		return;

	std::string title;
	std::vector<std::shared_ptr<IPropertyAction>> actions;
	if (! m_Editor->GetUIFrame().ExtendContextMenu(node, &title, &actions))
		return;

	/* Create actions to the context menu */
	QAction* nameAction = DEBUG_NEW QAction(title.c_str(), this);
	nameAction->setEnabled(false);

	/* Set the actions to the menu */
	menu->addAction(nameAction);
	menu->addSeparator();
	for (const auto& act : actions)
	{
		QAction * action = DEBUG_NEW QAction(act->GetName().c_str(), this);
		connect(action, &QAction::triggered, this, [act] {
			act->ExecuteAction();
		});
		menu->addAction(action);
	}

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
	m_Editor->GetTools().DropEvent(glm::vec2(event->pos().x(), event->pos().y()));
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
			m_Editor->GetTools().DragEnterEvent(data);
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
	m_Editor->GetTools().DragMoveEvent(glm::vec2(event->pos().x(), event->pos().y()));
}

void ZenonWindow3D::dragLeaveEvent(QDragLeaveEvent * event)
{
	m_Editor->GetTools().DragLeaveEvent();
}
