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

	QAction* uninstallAction = new QAction("Uninstall TA", m_SceneTreeViewerContextMenu.get());
	m_SceneTreeViewerContextMenu->addAction(uninstallAction);

	m_SceneTreeViewerContextMenu->addSeparator();

	QAction* uninstallAction33 = new QAction("Uninstall TA33", m_SceneTreeViewerContextMenu.get());
	m_SceneTreeViewerContextMenu->addAction(uninstallAction33);

	ui.SceneTreeViewer->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(ui.SceneTreeViewer, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

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

	CSceneNodeTreeModel * model = new CSceneNodeTreeModel(this);
	model->SetModelData(Scene);
	ui.SceneTreeViewer->setModel(model);

	ui.SceneTreeViewer->expandAll();
}

void MainEditor::ApplyTest()
{
	QFileSystemModel* fsModel = new QFileSystemModel(this);
	fsModel->setRootPath("D:\\_programming\\ZenonEngine\\gamedata");
	ui.FSTreeViewer->setModel(fsModel);

	m_PropertiesController->SceneNodeSelected(nullptr);
}


void MainEditor::onCustomContextMenu(const QPoint &point)
{
	QModelIndex index = ui.SceneTreeViewer->indexAt(point);
	if (index.isValid() && index.row() % 2 == 0) 
	{
		m_SceneTreeViewerContextMenu->exec(ui.SceneTreeViewer->viewport()->mapToGlobal(point));
	}
}