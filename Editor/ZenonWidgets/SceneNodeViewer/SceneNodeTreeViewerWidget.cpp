#include "stdafx.h"

// General
#include "SceneNodeTreeViewerWidget.h"

SceneNodeTreeViewerWidget::SceneNodeTreeViewerWidget(QWidget * parent)
	: QTreeView(parent)
	, m_Editor3D(nullptr)
	, m_EditorUI(nullptr)
	, m_SelectionBlocked(false)
{
	// Add context menu for scene node viewer
	m_SceneTreeViewerContextMenu = std::make_shared<QMenu>(this);
	m_SceneTreeViewerContextMenu->setTitle("Somec context menu title.");

	this->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

	// SceneNodeTreeView: Main settings
	m_SceneTreeViewerModel = std::make_shared<SceneNodeTreeModel>(this);
	this->setModel(m_SceneTreeViewerModel.get());

	// SceneNodeTreeView: Selection settings
	this->setSelectionMode(QAbstractItemView::SingleSelection);
	QItemSelectionModel* selectionModel = this->selectionModel();
	connect(selectionModel, SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onCurrentChanged(const QModelIndex&, const QModelIndex&)));

}

SceneNodeTreeViewerWidget::~SceneNodeTreeViewerWidget()
{
}

void SceneNodeTreeViewerWidget::RefreshTreeViewModel()
{
	m_Editor3D->LockUpdates();

	this->reset();
	m_SceneTreeViewerModel->SetModelData(m_Editor3D->GetRealRootNode3D());
	this->expandAll();

	m_Editor3D->UnlockUpdates();
}

void SceneNodeTreeViewerWidget::SelectNode(const std::shared_ptr<ISceneNode3D>& Node)
{
	const auto& index = m_SceneTreeViewerModel->Find(Node);
	scrollTo(index);
	selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
}


//
// Slots
//
void SceneNodeTreeViewerWidget::onCustomContextMenu(const QPoint& point)
{
	m_SelectionBlocked = true;

	QModelIndex index = indexAt(point);
	if (!index.isValid())
		return;

	CSceneNodeTreeItem* item = static_cast<CSceneNodeTreeItem*>(index.internalPointer());
	_ASSERT_EXPR(item != nullptr, L"Item is null.");

	QMenu * menu = new QMenu(this);

	m_EditorUI->ExtendContextMenu(menu, item->GetSceneNode());

	menu->popup(mapToGlobal(point));

	m_SelectionBlocked = false;
	
}

void SceneNodeTreeViewerWidget::onCurrentChanged(const QModelIndex& current, const QModelIndex& previous)
{
	if (!current.isValid())
		return;

	CSceneNodeTreeItem* item = static_cast<CSceneNodeTreeItem*>(current.internalPointer());
	_ASSERT_EXPR(item != nullptr, L"Item is null.");

	//m_EditorUI->OnSceneNodeSelectedIn3DEditor(item->GetSceneNode());
	m_Editor3D->OnSceneNodeSelectedInUIEditor(item->GetSceneNode());
}
