﻿#include "stdafx.h"

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


namespace
{
	void FillActionsList(const std::shared_ptr<ISceneNode3D>& Node, std::map<std::string, std::vector<std::shared_ptr<IPropertyAction>>> * actionsNames)
	{
		for (const auto& it : Node->GetProperties()->GetProperties())
		{
			if (auto act = std::dynamic_pointer_cast<IPropertyAction>(it.second))
			{
				std::vector<std::shared_ptr<IPropertyAction>> actions;
				actions.push_back(act);
				actionsNames->insert(std::make_pair(it.second->GetName(), actions));
			}
		}
	}

	void FillActionsMap(const std::shared_ptr<ISceneNode3D>& Node, std::map<std::string, std::shared_ptr<IPropertyAction>> * actionsNames)
	{
		for (const auto& it : Node->GetProperties()->GetProperties())
		{
			if (auto act = std::dynamic_pointer_cast<IPropertyAction>(it.second))
			{
				actionsNames->insert(std::make_pair(it.second->GetName(), act));
			}
		}
	}
}


//
// IEditorUIFrame
//
void MainEditor::ExtendContextMenu(QMenu * Menu, const std::shared_ptr<ISceneNode3D>& Node)
{
	std::string contextMenuTitle = Node->GetName();

	std::vector<QAction *> actions;

	if (Selector_GetSelectedNodes().size() > 1)
	{
		contextMenuTitle.append("and " + std::to_string(Selector_GetSelectedNodes().size()) + " nodes.");

		std::map<std::string, std::vector<std::shared_ptr<IPropertyAction>>> mainNodeActionsNames;
		FillActionsList(Node, &mainNodeActionsNames);

		for (const auto& sel : Selector_GetSelectedNodes())
		{
			if (sel == Node)
				continue;

			std::map<std::string, std::shared_ptr<IPropertyAction>> selNodeActionsNames;
			FillActionsMap(sel, &selNodeActionsNames);

			for (const auto& act : selNodeActionsNames)
			{
				auto it = std::find_if(mainNodeActionsNames.begin(), mainNodeActionsNames.end(), [&act](const std::pair<std::string, std::vector<std::shared_ptr<IPropertyAction>>>& mainNodeAction) -> bool {
					return mainNodeAction.first == act.first;
				});

				if (it == mainNodeActionsNames.end())
					continue;

				mainNodeActionsNames[act.first].push_back(act.second);
			}
		}

		for (const auto& it : mainNodeActionsNames)
		{
			std::string actionName = it.first;
			auto actionsList = it.second;
			if (actionsList.size() > 1)
				actionName.append(" [" + std::to_string(actionsList.size()) + "]");
			QAction * action = new QAction(actionName.c_str(), this);
			this->connect(action, &QAction::triggered, this, [actionsList] {
				for (const auto& act : actionsList)
					act->ExecuteAction();
			});
			actions.push_back(action);
		}
	}
	else
	{
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

	}

	/* Create actions to the context menu */
	QAction* nameAction = new QAction(contextMenuTitle.c_str(), this);
	nameAction->setEnabled(false);

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
