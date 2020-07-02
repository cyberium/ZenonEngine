#include "stdafx.h"

// General
#include "SceneNodeTreeViewerWidget.h"

SceneNodeTreeViewerWidget::SceneNodeTreeViewerWidget(QWidget * parent)
	: QTreeView(parent)
	, m_Editor3D(nullptr)
	, m_EditorUI(nullptr)
{
	// Add context menu for scene node viewer
	m_SceneTreeViewerContextMenu = std::make_shared<QMenu>(this);
	m_SceneTreeViewerContextMenu->setTitle("Somec context menu title.");
	this->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

	// SceneNodeTreeView: Main settings
	m_SceneTreeViewerModel = std::make_shared<CSceneNodeTreeModel>(this);
	this->setModel(m_SceneTreeViewerModel.get());

	// SceneNodeTreeView: Selection settings
	this->setSelectionMode(QAbstractItemView::SingleSelection);
	QItemSelectionModel* selectionModel = this->selectionModel();
	connect(selectionModel, SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onCurrentChanged(const QModelIndex&, const QModelIndex&)));

}

SceneNodeTreeViewerWidget::~SceneNodeTreeViewerWidget()
{
}

void SceneNodeTreeViewerWidget::RefreshTreeViewModel(const std::shared_ptr<ISceneNode3D>& SceneNode)
{
	m_SceneTreeViewerModel->SetModelData(m_Editor3D->GetRealRootNode3D());
	this->expandAll();
}


//
// Slots
//
void SceneNodeTreeViewerWidget::onCustomContextMenu(const QPoint& point)
{
	QModelIndex index = this->indexAt(point);
	if (!index.isValid())
	{
		return;
	}

	CSceneNodeTreeItem* item = static_cast<CSceneNodeTreeItem*>(index.internalPointer());
	_ASSERT_EXPR(item != nullptr, L"Item is null.");

	m_SceneTreeViewerContextMenu->clear();

	{
		QAction* nameAction = new QAction(item->GetSceneNode()->GetName().c_str(), m_SceneTreeViewerContextMenu.get());
		nameAction->setEnabled(false);
		m_SceneTreeViewerContextMenu->addAction(nameAction);

		m_SceneTreeViewerContextMenu->addSeparator();

		QAction* uninstallAction33 = new QAction("Uninstall TA33", m_SceneTreeViewerContextMenu.get());
		m_SceneTreeViewerContextMenu->addAction(uninstallAction33);
	}

	m_SceneTreeViewerContextMenu->exec(this->viewport()->mapToGlobal(point));
}

void SceneNodeTreeViewerWidget::onCurrentChanged(const QModelIndex& current, const QModelIndex& previous)
{
	if (!current.isValid())
		return;

	CSceneNodeTreeItem* item = static_cast<CSceneNodeTreeItem*>(current.internalPointer());
	_ASSERT_EXPR(item != nullptr, L"Item is null.");

	m_EditorUI->OnSceneNodeSelectedIn3DEditor(item->GetSceneNode());
	m_Editor3D->OnSceneNodeSelectedInUIEditor(item->GetSceneNode());
}
