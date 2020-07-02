#include "stdafx.h"

// General
#include "MainEditor.h"

#include <qfilesystemmodel.h>

MainEditor::MainEditor(QWidget* Parent)
	: QMainWindow(Parent)
{
	m_UI.setupUi(this);

	QFileSystemModel* fsModel = new QFileSystemModel(this);
	fsModel->setRootPath("D:\\_programming\\ZenonEngine\\gamedata");
	m_UI.FSTreeViewer->setModel(fsModel);

	m_PropertiesController = std::make_shared<CPropertiesController>(m_UI.PropertyEditor);

	// Unite file browser and log docker
	QMainWindow::tabifyDockWidget(m_UI.DockerFileBrowser, m_UI.DockerLog);
}

MainEditor::~MainEditor()
{
}



//
// IEditorUIFrame
//

void MainEditor::ExtendContextMenu(QMenu * Menu, const std::shared_ptr<ISceneNode3D>& Node)
{
	/* Create actions to the context menu */
	QAction* nameAction = new QAction(Node->GetName().c_str(), this);
	nameAction->setEnabled(false);

	std::vector<QAction *> actions;
	for (const auto& it : Node->GetActions()->GetActions())
	{
		QAction * action = new QAction(it.second->GetName().c_str(), this);
		connect(action, &QAction::triggered, this, [&it, Node] {
			it.second->ExecuteAction(Node);
		});
		actions.push_back(action);
	}

	/* Set the actions to the menu */
	Menu->addAction(nameAction);
	Menu->addSeparator();
	for (const auto& it : actions)
		Menu->addAction(it);
}

void MainEditor::OnSceneChanged()
{
	getSceneTree()->RefreshTreeViewModel();
}
void MainEditor::OnSceneNodeSelectedIn3DEditor(const std::shared_ptr<ISceneNode3D>& SceneNode3D)
{
	OnSceneNodeSelected(SceneNode3D);
}



//
// IEditorSharedFrame
//
void MainEditor::OnSceneNodeSelected(const std::shared_ptr<ISceneNode3D>& SceneNode3D)
{
	getSceneTree()->SelectNode(SceneNode3D);

	m_PropertiesController->OnSceneNodeSelected(SceneNode3D.get());
}
