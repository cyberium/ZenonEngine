#include "stdafx.h"

// General
#include "ZenonTreeViewWidget.h"

// Additional
#include "EditorUI/ContextMenuUtils.h"
#include "EditorUI/DragUtils.h"

ZenonTreeViewWidget::ZenonTreeViewWidget(QWidget * parent)
	: QTreeView(parent)
	, m_LockForSelectionChangedEvent(false)
	, m_IsDraggedNow(false)
	, m_DraggedTreeViewItem(nullptr)
{
	// Add context menu for scene node viewer
	m_ContextMenu = MakeShared(QMenu, this);
	m_ContextMenu->setTitle("Some context menu title.");

	this->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

	// SceneNodeTreeView: Main settings
	m_Model = MakeShared(CznTreeViewModel, this);
	this->setModel(m_Model.get());

	// SceneNodeTreeView: Selection settings
	//this->setSelectionMode(QAbstractItemView::ExtendedSelection);
	//this->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);

	QItemSelectionModel* selectionModel = this->selectionModel();
	connect(selectionModel, SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onCurrentChanged(const QModelIndex&, const QModelIndex&)));
	connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(onSelectionChanged(const QItemSelection&, const QItemSelection&)));

	connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onClicked(const QModelIndex&)));
}

ZenonTreeViewWidget::~ZenonTreeViewWidget()
{
}



void ZenonTreeViewWidget::Refresh()
{
	this->reset();
	m_Model->ClearRootCache();
	this->expandAll();
}

void ZenonTreeViewWidget::AddToRoot(const std::shared_ptr<IznTreeViewItem>& RootItem, bool DeleteExisting)
{
	m_LockForSelectionChangedEvent = true;

	{
		if (DeleteExisting)
			m_Model->ClearRoot();

		m_Model->AddToRoot(RootItem);
	}

	Refresh();

	m_LockForSelectionChangedEvent = false;
}

void ZenonTreeViewWidget::AddToRoot(const std::vector<std::shared_ptr<IznTreeViewItem>>& RootItems, bool DeleteExisting)
{
	m_LockForSelectionChangedEvent = true;
	
	{
		if (DeleteExisting)
			m_Model->ClearRoot();

		for (const auto& item : RootItems)
			m_Model->AddToRoot(item);
	}

	Refresh();

	m_LockForSelectionChangedEvent = false;
}

void ZenonTreeViewWidget::ClearSelection()
{
	m_LockForSelectionChangedEvent = true;
	selectionModel()->clear();
	m_LockForSelectionChangedEvent = false;
}

void ZenonTreeViewWidget::SelectItem(const std::shared_ptr<IObject>& Item, bool NeedClear)
{
	m_LockForSelectionChangedEvent = true;

	if (NeedClear)
		selectionModel()->clear();

	const auto& index = m_Model->Find(Item);
	scrollTo(index);
	selectionModel()->select(index, QItemSelectionModel::Select);

	m_LockForSelectionChangedEvent = false;
}

void ZenonTreeViewWidget::SelectItems(const std::vector<std::shared_ptr<IObject>>& Items)
{
	m_LockForSelectionChangedEvent = true;

	selectionModel()->clear();
	for (const auto& item : Items)
	{
		const auto& index = m_Model->Find(item);
		selectionModel()->select(index, QItemSelectionModel::Select);
		scrollTo(index);
	}

	m_LockForSelectionChangedEvent = false;
}



//
// Callbacks
//
void ZenonTreeViewWidget::SetOnContexMenu(OnContextMenuCallback Callback)
{
	m_OnContextMenu = Callback;
}

void ZenonTreeViewWidget::SetOnSelectionChange(OnSelectionChangeCallback Callback)
{
	m_OnSelectionChange = Callback;
}

void ZenonTreeViewWidget::SetOnSelectedItemChange(OnSelectedItemChangeCallback Callback)
{
	m_OnSelectedItemChange = Callback;
}

// Create Drag & Drop
void ZenonTreeViewWidget::SetOnStartDragging(OnDragStartCallback Callback)
{
	m_OnDragStart = Callback;
}

// Accept Drag & Drop
void ZenonTreeViewWidget::SetOnDragEnter(OnDragEnterCallback Callback)
{
	m_OnDragEnterCallback = Callback;
}

void ZenonTreeViewWidget::SetOnDragMove(OnDragMoveCallback Callback)
{
	m_OnDragMoveCallback = Callback;
}

void ZenonTreeViewWidget::SetOnDragDrop(OnDragDropCallback Callback)
{
	m_OnDragDropCallback = Callback;
}

void ZenonTreeViewWidget::SetOnDragLeave(OnDragLeaveCallback Callback)
{
	m_OnDragLeaveCallback = Callback;
}




//
// Protected
//
void ZenonTreeViewWidget::mousePressEvent(QMouseEvent * event)
{
	mousePressEventInternal(event);
	__super::mousePressEvent(event);
}

void ZenonTreeViewWidget::mouseReleaseEvent(QMouseEvent * event)
{
	mouseReleaseEventInternal(event);
	__super::mouseReleaseEvent(event);
}

void ZenonTreeViewWidget::mouseMoveEvent(QMouseEvent * event)
{
	mouseMoveEventInternal(event);
	__super::mouseMoveEvent(event);
}

void ZenonTreeViewWidget::dropEvent(QDropEvent * event)
{
	event->ignore();

	const QMimeData* mimeData = event->mimeData();
	if (mimeData == nullptr)
		return;

	const QByteArray qtByteBuffer = mimeData->data("ZenonEngineMimeData");
	if (qtByteBuffer.isEmpty())
		return;

	if (m_OnDragDropCallback == nullptr)
	{
		event->accept();
		return;
	}

	try
	{
		const QModelIndex index = indexAt(event->pos());
		if (false == index.isValid())
			return;

		const auto underCursorItem = static_cast<IznTreeViewItem*>(index.internalPointer());
		if (underCursorItem == nullptr)
			return;

		const CByteBuffer buffer(qtByteBuffer.data(), qtByteBuffer.size());

		if (false == m_OnDragDropCallback(underCursorItem, buffer))
			return;

		event->accept();
	}
	catch (const CException& e)
	{
		Log::Error("ZenonTreeViewWidget: Exception occurs in 'dropEvent'.");
		Log::Error("--->%s", e.MessageCStr());
	}
	catch (...)
	{
		Log::Error("ZenonTreeViewWidget: Unknown exception occurs in 'dropEvent'.");
	}
}

void ZenonTreeViewWidget::dragEnterEvent(QDragEnterEvent * event)
{
	event->ignore();

	const QMimeData * mimeData = event->mimeData();
	if (mimeData == nullptr)
		return;

	const QByteArray qtByteBuffer = mimeData->data("ZenonEngineMimeData");
	if (qtByteBuffer.isEmpty())
		return;

	if (m_OnDragEnterCallback == nullptr)
	{
		event->accept();
		return;
	}

	try
	{
		const CByteBuffer buffer(qtByteBuffer.data(), qtByteBuffer.size());

		if (false == m_OnDragEnterCallback(buffer))
			return;

		event->accept();
	}
	catch (const CException& e)
	{
		Log::Error("ZenonTreeViewWidget: Exception occurs in 'dragEnterEvent'.");
		Log::Error("--->%s", e.MessageCStr());
	}
	catch (...)
	{
		Log::Error("ZenonTreeViewWidget: Unknown exception occurs in 'dragEnterEvent'.");
	}
}

void ZenonTreeViewWidget::dragMoveEvent(QDragMoveEvent * event)
{
	event->ignore();

	const QMimeData * mimeData = event->mimeData();
	if (mimeData == nullptr)
		return;

	const QByteArray qtByteBuffer = mimeData->data("ZenonEngineMimeData");
	if (qtByteBuffer.isEmpty())
		return;

	if (m_OnDragMoveCallback == nullptr)
	{
		event->accept();
		return;
	}

	try
	{
		const QModelIndex index = indexAt(event->pos());
		if (false == index.isValid())
			return;

		const auto underCursorItem = static_cast<IznTreeViewItem*>(index.internalPointer());
		if (underCursorItem == nullptr)
			return;

		const CByteBuffer buffer(qtByteBuffer.data(), qtByteBuffer.size());

		if (false == m_OnDragMoveCallback(underCursorItem, buffer))
			return;

		event->accept();
	}
	catch (const CException& e)
	{
		Log::Error("ZenonTreeViewWidget: Exception occurs in 'dragMoveEvent'.");
		Log::Error("--->%s", e.MessageCStr());
	}
	catch (...)
	{
		Log::Error("ZenonTreeViewWidget: Unknown exception occurs in 'dragMoveEvent'.");
	}
}

void ZenonTreeViewWidget::dragLeaveEvent(QDragLeaveEvent * event)
{
	event->ignore();

	if (m_OnDragLeaveCallback == nullptr)
	{
		event->accept();
		return;
	}

	try
	{
		if (false == m_OnDragLeaveCallback())
			return;
		
		event->accept();
	}
	catch (const CException& e)
	{
		Log::Error("ZenonTreeViewWidget: Exception occurs in 'dragLeaveEvent'.");
		Log::Error("--->%s", e.MessageCStr());
	}
	catch (...)
	{
		Log::Error("ZenonTreeViewWidget: Unknown exception occurs in 'dragLeaveEvent'.");
	}
}



//
// Slots
//
void ZenonTreeViewWidget::onCustomContextMenu(const QPoint& point)
{
	if (m_LockForSelectionChangedEvent)
		return;

	QModelIndex index = indexAt(point);
	if (!index.isValid())
		return;

	auto item = static_cast<IznTreeViewItem*>(index.internalPointer());
	_ASSERT_EXPR(item != nullptr, L"Item is null.");

	m_ContextMenu->clear();

	if (m_OnContextMenu == nullptr)
		return;

	std::shared_ptr<IPropertiesGroup> propertiesGroup = MakeShared(CPropertiesGroup, "DefaultContextMenuTitle", "DefaultContextMenuDescription");
	if (false == m_OnContextMenu(item, propertiesGroup)) // TODO: try/catch
		return;

	m_ContextMenu = CreateContextMenuFromPropertiesGroup(propertiesGroup);
	m_ContextMenu->popup(mapToGlobal(point));
}

void ZenonTreeViewWidget::onCurrentChanged(const QModelIndex& current, const QModelIndex& previous)
{
	if (m_LockForSelectionChangedEvent)
		return;

	if (m_OnSelectedItemChange == nullptr)
		return;

	// TODO: try/catch
	m_OnSelectedItemChange(static_cast<IznTreeViewItem*>(current.internalPointer()));
}

void ZenonTreeViewWidget::onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
	if (m_LockForSelectionChangedEvent)
		return;

	if (m_OnSelectionChange == nullptr)
		return;

	std::vector<const IznTreeViewItem*> selectedNodes;
	auto indexes = selectionModel()->selectedIndexes();
	std::for_each(indexes.begin(), indexes.end(), [&selectedNodes](const QModelIndex& Index) {
		selectedNodes.push_back(static_cast<IznTreeViewItem*>(Index.internalPointer()));
	});

	// TODO: try/catch
	m_OnSelectionChange(selectedNodes);
}

void ZenonTreeViewWidget::onPressed(const QModelIndex& index)
{
}

void ZenonTreeViewWidget::onClicked(const QModelIndex& index)
{
	if (m_LockForSelectionChangedEvent)
		return;

	if (!index.isValid())
		return;
}

void ZenonTreeViewWidget::onDoubleClicked(const QModelIndex & index)
{

}



//
// Protected
//
void ZenonTreeViewWidget::mousePressEventInternal(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		QModelIndex index = indexAt(event->pos());
		if (false == index.isValid())
			return;

		auto item = static_cast<IznTreeViewItem*>(index.internalPointer());
		if (item == nullptr)
			return;

		m_IsDraggedNow = false;
		m_DraggedTreeViewItem = item;
		m_PrevioisMousePos = glm::ivec2(event->pos().x(), event->pos().y());
	}
}

void ZenonTreeViewWidget::mouseReleaseEventInternal(QMouseEvent * event)
{
	m_IsDraggedNow = false;
}

void ZenonTreeViewWidget::mouseMoveEventInternal(QMouseEvent * event)
{
	if (m_IsDraggedNow)
		return;

	if (m_DraggedTreeViewItem == nullptr)
		return;

	auto currentPos = glm::vec2(event->pos().x(), event->pos().y());
	float draggingOffsetFromStartDragging = glm::length(glm::abs(m_PrevioisMousePos - currentPos));
	if (draggingOffsetFromStartDragging <= 5.0f)
		return;

	if (m_OnDragStart == nullptr)
		return;

	CByteBuffer byteBuffer;
	try
	{
		if (false == m_OnDragStart(m_DraggedTreeViewItem, &byteBuffer))
			return;
	}
	catch (const CException& e)
	{
		Log::Error("ZenonTreeViewWidget: 'OnDragStart' exception.");
		Log::Error("--->%s", e.MessageCStr());
		m_IsDraggedNow = false;
		return;
	}

	m_IsDraggedNow = true;

	QMimeData* mimeData = ZN_NEW QMimeData();
	mimeData->setData("ZenonEngineMimeData", QByteArray((const char*)byteBuffer.getData(), byteBuffer.getSize()));

	QDrag* drag = ZN_NEW QDrag(this);
	std::string iconName = m_DraggedTreeViewItem->GetIconName();
	if (false == iconName.empty())
		if (QIcon* icon = m_Model->GetIcon(m_DraggedTreeViewItem->GetIconName()))
			drag->setPixmap(icon->pixmap(QSize(32, 32)));
	drag->setMimeData(mimeData);

	Qt::DropAction dropAction = drag->exec();
	if (dropAction != Qt::DropAction::IgnoreAction)
	{
		if (dropAction == Qt::DropAction::CopyAction)
			drag->cancel();
	}
}