#include "stdafx.h"

// General
#include "ZenonTreeViewWidget.h"

ZenonTreeViewWidget::ZenonTreeViewWidget(QWidget * parent)
	: QTreeView(parent)
	, m_LockForSelectionChangedEvent(false)
	, m_StartDragging(false)
{
	// Add context menu for scene node viewer
	m_ContextMenu = MakeShared(QMenu, this);
	m_ContextMenu->setTitle("Some context menu title.");

	this->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

	// SceneNodeTreeView: Main settings
	m_Model = MakeShared(CznQTTreeViewModel, this);
	this->setModel(m_Model.get());

	// SceneNodeTreeView: Selection settings
	this->setSelectionMode(QAbstractItemView::ExtendedSelection);
	this->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);

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

void ZenonTreeViewWidget::AddToRoot(const std::shared_ptr<IznTreeViewItemSource>& RootItem, bool DeleteExisting)
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

void ZenonTreeViewWidget::AddToRoot(const std::vector<std::shared_ptr<IznTreeViewItemSource>>& RootItems, bool DeleteExisting)
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

/*void ZenonTreeViewWidget::SetRootItems(const std::vector<std::shared_ptr<IznTreeViewItemSource>>& RootItems)
{
	m_LockForSelectionChangedEvent = true;

	this->reset();
	m_Model->SetChildRootItemsData(RootItems);
	this->expandAll();

	m_LockForSelectionChangedEvent = false;
}*/

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

void ZenonTreeViewWidget::SetOnStartDragging(OnDragStartCallback Callback)
{
	m_OnDragStart = Callback;
}

void ZenonTreeViewWidget::SetOnSelectionChange(OnSelectionChangeCallback Callback)
{
	m_OnSelectionChange = Callback;
}

void ZenonTreeViewWidget::SetOnSelectedItemChange(OnSelectedItemChangeCallback Callback)
{
	m_OnSelectedItemChange = Callback;
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

	auto item = static_cast<CznTreeViewItem*>(index.internalPointer());
	_ASSERT_EXPR(item != nullptr, L"Item is null.");

	m_ContextMenu->clear();

	if (m_OnContextMenu == nullptr)
		return;

	std::string title;
	std::vector<std::shared_ptr<IPropertyAction>> actions;
	if (false == m_OnContextMenu(item, &title, &actions))
		return;

	// Create actions to the context menu 
	QAction* nameAction = ZN_NEW QAction(title.c_str(), this);
	nameAction->setEnabled(false);

	// Set the actions to the menu
	m_ContextMenu->addAction(nameAction);
	m_ContextMenu->addSeparator();
	for (const auto& act : actions)
	{
		QAction * action = ZN_NEW QAction(act->GetName().c_str(), this);
		connect(action, &QAction::triggered, this, [act] {
			act->ExecuteAction();
		});
		m_ContextMenu->addAction(action);
	}

	m_ContextMenu->popup(mapToGlobal(point));
}

void ZenonTreeViewWidget::onCurrentChanged(const QModelIndex& current, const QModelIndex& previous)
{
	if (m_LockForSelectionChangedEvent)
		return;

	if (m_OnSelectedItemChange == nullptr)
		return;

	m_OnSelectedItemChange(static_cast<CznTreeViewItem*>(current.internalPointer()));
}

void ZenonTreeViewWidget::onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
	if (m_LockForSelectionChangedEvent)
		return;

	if (m_OnSelectionChange == nullptr)
		return;

	std::vector<const CznTreeViewItem*> selectedNodes;
	auto indexes = selectionModel()->selectedIndexes();
	std::for_each(indexes.begin(), indexes.end(), [&selectedNodes](const QModelIndex& Index) {
		selectedNodes.push_back(static_cast<CznTreeViewItem*>(Index.internalPointer()));
	});

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
		m_StartDragging = false;
		m_PrevioisMousePos = glm::ivec2(event->pos().x(), event->pos().y());
	}
}

void ZenonTreeViewWidget::mouseReleaseEventInternal(QMouseEvent * event)
{
	m_StartDragging = false;
}

void ZenonTreeViewWidget::mouseMoveEventInternal(QMouseEvent * event)
{
	if (m_StartDragging)
		return;

	auto currentPos = glm::vec2(event->pos().x(), event->pos().y());
	float draggingOffsetFromStartDragging = glm::length(glm::abs(m_PrevioisMousePos - currentPos));
	if (draggingOffsetFromStartDragging <= 5.0f)
		return;

	QModelIndex index = indexAt(event->pos());
	if (false == index.isValid())
		return;

	auto item = static_cast<CznTreeViewItem*>(index.internalPointer());
	if (item == nullptr)
		return;

	if (m_OnDragStart == nullptr)
		return;

	std::string mimeDataText;
	if (false == m_OnDragStart(item, &mimeDataText))
		return;

	m_StartDragging = true;

	QMimeData *mimeData = ZN_NEW QMimeData;
	mimeData->setText(mimeDataText.c_str());

	auto drag = ZN_NEW QDrag(this);
	drag->setMimeData(mimeData);
	drag->setHotSpot(event->pos());

	Qt::DropAction dropAction = drag->exec();
	if (dropAction != Qt::DropAction::IgnoreAction)
	{
		if (dropAction == Qt::DropAction::CopyAction)
			drag->cancel();
	}
}