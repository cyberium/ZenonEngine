#include "stdafx.h"

// General
#include "MainEditor.h"

// Addtionals
#include "SceneNodeTreeModel.h"
#include "RenderWindowWidget.h"

#include <qfilesystemmodel.h>

MainEditor::MainEditor(QWidget* Parent)
	: QMainWindow(Parent)
{
	ui.setupUi(this);


	m_PropertiesController = std::make_shared<CPropertiesController>(ui.PropertyEditor);

	// Add context menu for scene node viewer
	m_SceneTreeViewerContextMenu = std::make_shared<QMenu>(this);
	m_SceneTreeViewerContextMenu->setTitle("Somec context menu title.");
	ui.SceneTreeViewer->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.SceneTreeViewer, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));
	
	// SceneNodeTreeView: Main settings
	m_SceneTreeViewerModel = new CSceneNodeTreeModel(this);
	ui.SceneTreeViewer->setModel(m_SceneTreeViewerModel);

	// SceneNodeTreeView: Selection settings
	ui.SceneTreeViewer->setSelectionMode(QAbstractItemView::SingleSelection);
	QItemSelectionModel* selectionModel = ui.SceneTreeViewer->selectionModel();
	connect(selectionModel, SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onCurrentChanged(const QModelIndex&, const QModelIndex&)));

	// Unite file browser and log docker
	QMainWindow::tabifyDockWidget(ui.DockerFileBrowser, ui.DockerLog);
}

MainEditor::~MainEditor()
{
}

void MainEditor::OnSceneNodeSelected(std::shared_ptr<ISceneNode> SceneNode)
{
	m_PropertiesController->SceneNodeSelected(SceneNode);
}

void MainEditor::ApplyScene(std::shared_ptr<IScene> Scene)
{
	m_Scene = Scene;

	m_SceneTreeViewerModel->SetModelData(Scene);

	ui.SceneTreeViewer->expandAll();
}

void MainEditor::ApplyTest()
{
	QFileSystemModel* fsModel = new QFileSystemModel(this);
	fsModel->setRootPath("D:\\_programming\\ZenonEngine\\gamedata");
	ui.FSTreeViewer->setModel(fsModel);

	m_PropertiesController->SceneNodeSelected(nullptr);
}


//
// Slots
//
void MainEditor::onCustomContextMenu(const QPoint& point)
{
	QModelIndex index = ui.SceneTreeViewer->indexAt(point);
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

	m_SceneTreeViewerContextMenu->exec(ui.SceneTreeViewer->viewport()->mapToGlobal(point));
}

void MainEditor::onCurrentChanged(const QModelIndex& current, const QModelIndex& previous)
{
	if (!current.isValid())
	{
		return;
	}

	CSceneNodeTreeItem* item = static_cast<CSceneNodeTreeItem*>(current.internalPointer());
	_ASSERT_EXPR(item != nullptr, L"Item is null.");

	OnSceneNodeSelected(item->GetSceneNode());
}
