#include "stdafx.h"

// General
#include "ZenonSceneViewerWidget.h"

ZenonSceneViewerWidget::ZenonSceneViewerWidget(QWidget * parent)
	: QTreeView(parent)
	, m_Editor(nullptr)
	, m_LockForSelectionChangedEvent(false)
{
	// Add context menu for scene node viewer
	m_ContextMenu = MakeShared(QMenu, this);
	m_ContextMenu->setTitle("Some context menu title.");

	this->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

	// SceneNodeTreeView: Main settings
	//m_Model = MakeShared(CQtToZenonTreeModel, this);
	//this->setModel(m_Model.get());



	// SceneNodeTreeView: Selection settings
	this->setSelectionMode(QAbstractItemView::ExtendedSelection);
	this->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);

	QItemSelectionModel* selectionModel = this->selectionModel();
	connect(selectionModel, SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onCurrentChanged(const QModelIndex&, const QModelIndex&)));
	connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(onSelectionChanged(const QItemSelection&, const QItemSelection&)));

	connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onClicked(const QModelIndex&)));
}

ZenonSceneViewerWidget::~ZenonSceneViewerWidget()
{
}



#include "SceneBrowser/SceneBrowserNode.h"

namespace
{
	void FillWithChilds(const std::shared_ptr<CSceneBrowserTreeModel>& Model, const std::shared_ptr<ISceneNode3D>& Parent)
	{
		for (const auto& child : Parent->GetChilds())
		{
			Model->AddChildToParent(MakeShared(CznSceneBrowserNode, child), MakeShared(CznSceneBrowserNode, Parent));

			FillWithChilds(Model, child);
		}
	}
}


void ZenonSceneViewerWidget::SetEditor(IEditor* Editor) 
{ 
	m_Editor = Editor; 

}

void ZenonSceneViewerWidget::RefreshTreeViewModel(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode3D>& ParentNode, const std::shared_ptr<ISceneNode3D>& ChildNode)
{
	if (m_LockForSelectionChangedEvent)
		return;

	m_Editor->Get3DFrame().LockUpdates();

	m_Model = MakeShared(CSceneBrowserTreeModel, MakeShared(CznSceneBrowserNode, m_Editor->Get3DFrame().GetEditedRootNode3D()), nullptr);
	this->setModel(m_Model.get());

	this->reset();
	FillWithChilds(m_Model, m_Editor->Get3DFrame().GetEditedRootNode3D());
	//m_Model->SetRootItemData(MakeShared(CSceneNodeModelItem, m_Editor->Get3DFrame().GetEditedRootNode3D()));
	this->expandAll();

	m_Editor->Get3DFrame().UnlockUpdates();
}

void ZenonSceneViewerWidget::SelectNode(const std::shared_ptr<ISceneNode3D>& Node)
{
	m_LockForSelectionChangedEvent = true;
	//const auto& index = m_Model->Find(Node);
	//scrollTo(index);
	//selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
	m_LockForSelectionChangedEvent = false;
}

void ZenonSceneViewerWidget::SelectNodes(const std::vector<std::shared_ptr<ISceneNode3D>>& Nodes)
{
	m_LockForSelectionChangedEvent = true;
	//selectionModel()->clear();
	//for (const auto& node : Nodes)
	//{
	//	const auto& index = m_Model->Find(node);
	//	selectionModel()->select(index, QItemSelectionModel::Select);
	//}
	m_LockForSelectionChangedEvent = false;
}

void ZenonSceneViewerWidget::mousePressEvent(QMouseEvent * event)
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
void ZenonSceneViewerWidget::onCustomContextMenu(const QPoint& point)
{
	/*
	if (m_LockForSelectionChangedEvent)
		return;

	QModelIndex index = indexAt(point);
	if (!index.isValid())
		return;

	auto item = static_cast<CQtToZenonTreeItem*>(index.internalPointer());
	_ASSERT_EXPR(item != nullptr, L"Item is null.");

	m_ContextMenu->clear();

	std::string title;
	std::vector<std::shared_ptr<IPropertyAction>> actions;
	if (!m_Editor->GetUIFrame().ExtendContextMenu(std::dynamic_pointer_cast<ISceneNode3D>(item->GetTObject()), &title, &actions))
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
	*/
}

void ZenonSceneViewerWidget::onCurrentChanged(const QModelIndex& current, const QModelIndex& previous)
{

}

void ZenonSceneViewerWidget::onSelectionChanged(const QItemSelection& selected, const QItemSelection &deselected)
{
	if (m_LockForSelectionChangedEvent)
		return;

	/*std::vector<std::shared_ptr<ISceneNode3D>> selectedNodes;
	auto indexes = selectionModel()->selectedIndexes();
	std::for_each(indexes.begin(), indexes.end(), [this, &selectedNodes](const QModelIndex& Index) {
		if (auto node = m_Model->Find(Index))
			selectedNodes.push_back(std::static_pointer_cast<ISceneNode3D>(node));
	});
	indexes.clear();*/

	m_Editor->Get3DFrame().LockUpdates();
	//auto& selector = dynamic_cast<IEditorToolSelector&>(m_Editor->GetTools().GetTool(ETool::EToolSelector));
	//selector.SelectNodes(selectedNodes);
	m_Editor->Get3DFrame().UnlockUpdates();
}

void ZenonSceneViewerWidget::onPressed(const QModelIndex& index)
{
}

void ZenonSceneViewerWidget::onClicked(const QModelIndex& index)
{
	if (!index.isValid())
		return;

	/*
	auto item = static_cast<CQtToZenonTreeItem*>(index.internalPointer());
	_ASSERT_EXPR(item != nullptr, L"Item is null.");

	m_Editor->Get3DFrame().LockUpdates();
	auto& selector = dynamic_cast<IEditorToolSelector&>(m_Editor->GetTools().GetTool(ETool::EToolSelector));
	selector.SelectNode(std::static_pointer_cast<ISceneNode3D>(item->GetTObject()));
	m_Editor->Get3DFrame().UnlockUpdates();
	*/
}

void ZenonSceneViewerWidget::onDoubleClicked(const QModelIndex & index)
{

}
