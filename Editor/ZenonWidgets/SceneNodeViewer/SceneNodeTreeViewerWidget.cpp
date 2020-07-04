#include "stdafx.h"

// General
#include "SceneNodeTreeViewerWidget.h"

// Additional
#include "SceneNodesSelector.h"

SceneNodeTreeViewerWidget::SceneNodeTreeViewerWidget(QWidget * parent)
	: QTreeView(parent)
	, m_Editor3D(nullptr)
	, m_EditorUI(nullptr)
	, m_LockForSelectionChangedEvent(false)
{
	// Add context menu for scene node viewer
	m_SceneTreeViewerContextMenu = std::make_shared<QMenu>(this);
	m_SceneTreeViewerContextMenu->setTitle("Somec context menu title.");

	this->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

	// SceneNodeTreeView: Main settings
	m_Model = std::make_shared<SceneNodeTreeModel>(this);
	this->setModel(m_Model.get());

	// SceneNodeTreeView: Selection settings
	this->setSelectionMode(QAbstractItemView::ExtendedSelection);
	QItemSelectionModel* selectionModel = this->selectionModel();
	this->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);
	connect(selectionModel, SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onCurrentChanged(const QModelIndex&, const QModelIndex&)));
	connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(onSelectionChanged(const QItemSelection&, const QItemSelection&)));

	connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onClicked(const QModelIndex&)));
}

SceneNodeTreeViewerWidget::~SceneNodeTreeViewerWidget()
{
}

void SceneNodeTreeViewerWidget::RefreshTreeViewModel()
{
	if (m_LockForSelectionChangedEvent)
		return;

	m_Editor3D->LockUpdates();

	this->reset();
	m_Model->SetModelData(m_Editor3D->GetRealRootNode3D());
	this->expandAll();

	m_Editor3D->UnlockUpdates();
}

void SceneNodeTreeViewerWidget::SelectNode(const std::shared_ptr<ISceneNode3D>& Node)
{
	m_LockForSelectionChangedEvent = true;
	const auto& index = m_Model->Find(Node);
	scrollTo(index);
	selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
	m_LockForSelectionChangedEvent = false;
}

void SceneNodeTreeViewerWidget::SelectNodes(const std::vector<std::shared_ptr<ISceneNode3D>>& Nodes)
{
	m_LockForSelectionChangedEvent = true;
	selectionModel()->clear();
	for (const auto& node : Nodes)
	{
		const auto& index = m_Model->Find(node);
		selectionModel()->select(index, QItemSelectionModel::Select);
	}
	m_LockForSelectionChangedEvent = false;
}

void SceneNodeTreeViewerWidget::mousePressEvent(QMouseEvent * event)
{
	if (m_LockForSelectionChangedEvent)
		return;

	if (event->button() == Qt::RightButton)
		return;

	__super::mousePressEvent(event);
}


//
// Slots
//
void SceneNodeTreeViewerWidget::onCustomContextMenu(const QPoint& point)
{
	if (m_LockForSelectionChangedEvent)
		return;

	QModelIndex index = indexAt(point);
	if (!index.isValid())
		return;

	CSceneNodeTreeItem* item = static_cast<CSceneNodeTreeItem*>(index.internalPointer());
	_ASSERT_EXPR(item != nullptr, L"Item is null.");

	m_SceneTreeViewerContextMenu->clear();
	m_EditorUI->ExtendContextMenu(m_SceneTreeViewerContextMenu.get(), item->GetSceneNode());

	m_SceneTreeViewerContextMenu->popup(mapToGlobal(point));
}

void SceneNodeTreeViewerWidget::onCurrentChanged(const QModelIndex& current, const QModelIndex& previous)
{

}

void SceneNodeTreeViewerWidget::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	if (m_LockForSelectionChangedEvent)
		return;

	std::vector<std::shared_ptr<ISceneNode3D>> selectedNodes;
	auto indexes = selectionModel()->selectedIndexes();
	std::for_each(indexes.begin(), indexes.end(), [this, &selectedNodes](const QModelIndex& Index) {
		if (auto node = m_Model->Find(Index))
			selectedNodes.push_back(node);
	});
	indexes.clear();

	m_Editor3D->LockUpdates();
	dynamic_cast<CSceneNodesSelector*>(m_Editor3D)->Selector_SelectNodes(selectedNodes, false);
	m_Editor3D->UnlockUpdates();
}

void SceneNodeTreeViewerWidget::onPressed(const QModelIndex & index)
{
}

void SceneNodeTreeViewerWidget::onClicked(const QModelIndex & index)
{
	if (!index.isValid())
		return;

	CSceneNodeTreeItem* item = static_cast<CSceneNodeTreeItem*>(index.internalPointer());
	_ASSERT_EXPR(item != nullptr, L"Item is null.");

	m_Editor3D->LockUpdates();
	dynamic_cast<CSceneNodesSelector*>(m_Editor3D)->Selector_SelectNode(item->GetSceneNode(), false);
	m_Editor3D->UnlockUpdates();
}

void SceneNodeTreeViewerWidget::onDoubleClicked(const QModelIndex & index)
{
}
