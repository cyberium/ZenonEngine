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
	QMainWindow::tabifyDockWidget(m_UI.DockerFileBrowser, m_UI.DockerLogViewer);

	
}

MainEditor::~MainEditor()
{
}


void MainEditor::SetEditor3D(IEditor3DFrame* Editor3DFrame)
{
	m_Editor3D = Editor3DFrame;
	getMainEditor()->SetEditors(Editor3DFrame, this);
	getSceneViewer()->SetEditors(Editor3DFrame, this);
	getCollectionViewer()->SetEditors(Editor3DFrame, this);
	Selector_SetOtherSelector(dynamic_cast<CSceneNodesSelector*>(m_Editor3D));

	getCollectionViewer()->SetModelsList(Utils::GetAllFilesInDirectory("C:\\_engine\\ZenonEngine_gamedata\\models", ".znmdl"));
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
	for (const auto& it : Node->GetProperties()->GetProperties())
	{
		if (auto act = std::dynamic_pointer_cast<IPropertyAction>(it.second))
		{
			QAction * action = new QAction(it.second->GetName().c_str(), this);
			connect(action, &QAction::triggered, this, [act] {
				act->ExecuteAction();
			});
			actions.push_back(action);
		}
	}

	/* Set the actions to the menu */
	Menu->addAction(nameAction);
	Menu->addSeparator();
	for (const auto& it : actions)
		Menu->addAction(it);
}

void MainEditor::OnSceneChanged()
{
	getSceneViewer()->RefreshTreeViewModel();
}



//
// CSceneNodesSelector
//
void MainEditor::Selector_OnSelectionChange()
{
	const auto& selectedNodes = Selector_GetSelectedNodes();
	if (selectedNodes.size() == 1)
		getSceneViewer()->SelectNode(Selector_GetFirstSelectedNode());

	getSceneViewer()->SelectNodes(selectedNodes);

	m_PropertiesController->OnSceneNodeSelected(Selector_GetFirstSelectedNode().get());
}
