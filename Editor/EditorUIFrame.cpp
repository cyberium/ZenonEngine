﻿#include "stdafx.h"

// General
#include "EditorUIFrame.h"

CEditorUIFrame::CEditorUIFrame(IEditor& Editor)
	: QMainWindow(nullptr)
	, m_Editor(Editor)
{
	dynamic_cast<IEditorPrivate&>(m_Editor).SetUIFrame(this);

	m_UI.setupUi(this);

	// Quit
	connect(m_UI.actionOpen_Scene, &QAction::triggered, [this]() {

		std::string fileName = GetEditor().GetShell().ShowLoadFileDialog("");
		if (fileName.empty())
			return;

		auto file = m_Editor.GetBaseManager().GetManager<IFilesManager>()->Open(fileName);
		if (file == nullptr)
			return;

		CXMLManager xml(m_Editor.GetBaseManager());
		auto reader = xml.CreateReader(file);

		auto currentRoot = m_Editor.Get3DFrame().GetEditedRootNode3D();
		while (false == currentRoot->GetChilds().empty())
			currentRoot->RemoveChild(currentRoot->GetChilds()[0]);

		auto rootNodeXML = m_Editor.GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->LoadSceneNode3DXML(reader->GetChilds()[0], currentRoot->GetScene(), currentRoot->GetParent().lock());

		while (false == rootNodeXML->GetChilds().empty())
			currentRoot->AddChild(rootNodeXML->GetChilds()[0]);
	});

	// Save action
	connect(m_UI.actionSave_Scene, &QAction::triggered, [this]() {
		std::string fileName = GetEditor().GetShell().ShowSaveFileDialog("D:/Scene.xml");
		if (fileName.empty())
			return;

		CXMLManager manager(m_Editor.GetBaseManager());

		auto rootWriter = m_Editor.GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->SaveSceneNode3DXML(m_Editor.Get3DFrame().GetEditedRootNode3D());

		auto writer = manager.CreateWriter();
		writer->AddChild(rootWriter);

		auto file = manager.SaveWriterToFile(writer, fileName);
		file->Save();
	});

	QFileSystemModel* fsModel = ZN_NEW QFileSystemModel(this);
	fsModel->setRootPath("O:\\ZenonEngine_gamedata\\");
	m_UI.FSTreeViewer->setModel(fsModel);

	m_PropertiesController = MakeShared(CPropertiesController, m_UI.PropertyEditor);

	// Unite file browser and log docker
	QMainWindow::tabifyDockWidget(m_UI.DockerFileBrowser, m_UI.DockerLogViewer);
}

CEditorUIFrame::~CEditorUIFrame()
{
}

IEditor& CEditorUIFrame::GetEditor() const
{
	return m_Editor;
}

bool CEditorUIFrame::InitializeEditorFrame()
{
	getMainEditor()->SetEditor(&m_Editor);
	getSceneViewer()->SetEditor(&m_Editor);
	getCollectionViewer()->SetEditor(&m_Editor);

	std::vector<std::string> realNames;

	auto gameDataStorage = m_Editor.GetBaseManager().GetManager<IFilesManager>()->GetStorage(EFilesStorageType::GAMEDATA);
	auto fileNames = gameDataStorage->GetAllFilesInFolder("models", ".fbx");
	for (const auto& fbxFileName : fileNames)
	{
		try
		{
			auto filePtr = gameDataStorage->OpenFile(fbxFileName);
			filePtr->ChangeExtension("znmdl");

			if (m_Editor.GetBaseManager().GetManager<IFilesManager>()->IsFileExists(filePtr->Path_Name()))
			{
				realNames.push_back(filePtr->Path_Name());
				continue;
			}

			auto fbxModel = m_Editor.GetBaseManager().GetManager<IznModelsFactory>()->LoadModel(fbxFileName);
			auto znModelFile = m_Editor.GetBaseManager().GetManager<IznModelsFactory>()->SaveModel(fbxModel, filePtr->Path_Name());
			znModelFile->Save();
			realNames.push_back(znModelFile->Path_Name());
		}
		catch (const CException& e)
		{
			Log::Error(e.MessageCStr());
		}
	}

	getCollectionViewer()->SetModelsList(realNames);

	return false;
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
void CEditorUIFrame::DoInitializeToolsUI()
{
	GetEditor().GetTools().DoInitializeUI(*this);
}

bool CEditorUIFrame::ExtendContextMenu(const std::shared_ptr<ISceneNode3D>& Node, std::string * Title, std::vector<std::shared_ptr<IPropertyAction>> * Actions)
{
	_ASSERT(Title != NULL);
	_ASSERT(Actions != NULL);
	_ASSERT(Actions->empty());

	if (!GetEditor().IsNodeSelected(Node))
		return false;

	*Title = Node->GetName();

	std::vector<QAction *> actions;

	if (GetEditor().GetSelectedNodes().size() > 1)
	{
		Title->append(" and " + std::to_string(GetEditor().GetSelectedNodes().size()) + " nodes.");

		std::map<std::string, std::vector<std::shared_ptr<IPropertyAction>>> mainNodeActionsNames;
		FillActionsList(Node, &mainNodeActionsNames);

		for (const auto& sel : GetEditor().GetSelectedNodes())
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

			std::shared_ptr<IPropertyAction> complexAction = MakeShared(CAction, actionName, "");
			complexAction->SetAction([actionsList] () -> bool {
				for (const auto& act : actionsList)
					act->ExecuteAction();
				return true;
			});

			Actions->push_back(complexAction);
		}
	}
	else
	{
		for (const auto& it : Node->GetProperties()->GetProperties())
		{
			if (auto act = std::dynamic_pointer_cast<IPropertyAction>(it.second))
			{
				Actions->push_back(act);
			}
		}
	}

	return true;
}

void CEditorUIFrame::OnSceneChanged(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode3D>& ParentNode, const std::shared_ptr<ISceneNode3D>& ChildNode)
{
	getSceneViewer()->RefreshTreeViewModel(SceneChangeType, ParentNode, ChildNode);
}



//
// IEditorQtUIFrame
//
QObject& CEditorUIFrame::getQObject()
{
	return *this;
}

HWND CEditorUIFrame::getHWND()
{
	return (HWND)winId();;
}

Ui::EditorUIFrameClass& CEditorUIFrame::getUI()
{
	return m_UI;
}



//
// IEditorToolSelectorEventListener
//
void CEditorUIFrame::OnSelectNode()
{
	const auto& selectedNodes = GetEditor().GetSelectedNodes();
	if (selectedNodes.size() == 1)
		getSceneViewer()->SelectNode(GetEditor().GetFirstSelectedNode());

	getSceneViewer()->SelectNodes(selectedNodes);

	//m_PropertiesController->OnSceneNodeSelected(GetEditor().GetFirstSelectedNode().get());
}
