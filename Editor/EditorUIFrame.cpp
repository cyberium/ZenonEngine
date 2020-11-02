#include "stdafx.h"

// General
#include "EditorUIFrame.h"


CEditorUIFrame::CEditorUIFrame(IEditor& Editor)
	: QMainWindow(nullptr)
	, m_Editor(Editor)
	, m_EditorResourceBrowser(Editor)
{
	dynamic_cast<IEditorPrivate&>(m_Editor).SetUIFrame(this);

	m_UI.setupUi(this);

	// Load & Save
	connect(m_UI.actionOpen_Scene, &QAction::triggered, std::bind(&CEditorUIFrame::OnSceneLoadFromFile, this));
	connect(m_UI.actionSave_Scene, &QAction::triggered, std::bind(&CEditorUIFrame::OnSceneSaveToFile, this));
	connect(m_UI.actionClose_Scene, &QAction::triggered, std::bind(&CEditorUIFrame::OnSceneClose, this));

	//QFileSystemModel* fsModel = ZN_NEW QFileSystemModel(this);
	//fsModel->setRootPath("O:\\ZenonEngine_gamedata\\");
	//m_UI.FSTreeViewer->setModel(fsModel);

	m_PropertiesController = MakeShared(CPropertiesController, m_UI.PropertyEditor);

	// Unite file browser and log docker
	//QMainWindow::tabifyDockWidget(m_UI.DockerFileBrowser, m_UI.DockerLogViewer);
}

CEditorUIFrame::~CEditorUIFrame()
{
}




namespace
{
	void FillActionsList(const std::shared_ptr<ISceneNode>& Node, std::map<std::string, std::vector<std::shared_ptr<IPropertyAction>>> * actionsNames)
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

	void FillActionsMap(const std::shared_ptr<ISceneNode>& Node, std::map<std::string, std::shared_ptr<IPropertyAction>> * actionsNames)
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

IEditor& CEditorUIFrame::GetEditor() const
{
	return m_Editor;
}

bool CEditorUIFrame::InitializeEditorFrame()
{
	getMainEditor()->SetEditor(&m_Editor);

	m_EditorResourceBrowser.Initialize();
	m_EditorResourceBrowser.InitializeSceneBrowser();

	return true;
}

void CEditorUIFrame::DoInitializeToolsUI()
{
	GetEditor().GetTools().DoInitializeUI(*this);
}

bool CEditorUIFrame::ExtendContextMenu(const std::shared_ptr<ISceneNode>& Node, std::string * Title, std::vector<std::shared_ptr<IPropertyAction>> * Actions)
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

void CEditorUIFrame::OnSceneChanged(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode>& ParentNode, const std::shared_ptr<ISceneNode>& ChildNode)
{
	if (SceneChangeType == ESceneChangeType::NodeRemovedFromParent)
	{
		m_Editor.GetTools().GetToolT<IEditorToolSelector>(ETool::EToolSelector).RemoveNode(ChildNode);
	}

	m_EditorResourceBrowser.UpdateSceneBrowser();
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
	return (HWND)winId();
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
	if (selectedNodes.empty())
	{
		getSceneViewer()->ClearSelection();
	}
	if (selectedNodes.size() == 1)
	{
		getSceneViewer()->SelectItem(GetEditor().GetFirstSelectedNode());
	}
	else
	{
		getSceneViewer()->ClearSelection();
		for (const auto& selectedNode : selectedNodes)
			getSceneViewer()->SelectItem(selectedNode, false);
	}

	m_PropertiesController->OnSceneNodeSelected(GetEditor().GetFirstSelectedNode().get());
}



//
// Protected
//
void CEditorUIFrame::OnSceneLoadFromFile()
{
	std::string fileName = GetEditor().GetShell().ShowLoadFileDialog("");
	if (fileName.empty())
		return;

	auto file = m_Editor.GetBaseManager().GetManager<IFilesManager>()->Open(fileName);
	if (file == nullptr)
		return;

	OnSceneClose();

	auto editorRoot = m_Editor.Get3DFrame().GetEditedRootNode3D();
	auto realRoot = editorRoot->GetParent(); // Temporary we add nodes to real root

	CXMLManager xml(m_Editor.GetBaseManager());
	auto reader = xml.CreateReader(file);
	auto xmlRoot = m_Editor.GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->LoadSceneNode3DXML(reader->GetChilds()[0], editorRoot->GetScene(), realRoot);

	// Update persistance nodes
	for (const auto& editorChild : editorRoot->GetChilds())
	{
		if (editorChild->IsPersistance())
		{
			auto xmlChild = xmlRoot->GetChild(editorChild->GetName());
			if (xmlChild != nullptr)
			{
				xmlChild->CopyTo(editorChild);

				// To delete persistance node, we must clear this flag
				std::dynamic_pointer_cast<ISceneNodeInternal>(xmlChild)->SetPersistanceInternal(false);
				xmlRoot->RemoveChild(xmlChild);
			}
		}
	}

	// Add new childs
	while (false == xmlRoot->GetChilds().empty())
	{
		auto xmlChild = *(xmlRoot->GetChilds().begin());
		editorRoot->AddChild(xmlChild);
	}
}

void CEditorUIFrame::OnSceneSaveToFile()
{
	std::string fileName = GetEditor().GetShell().ShowSaveFileDialog("");
	if (fileName.empty())
		return;

	CXMLManager manager(m_Editor.GetBaseManager());

	auto rootWriter = m_Editor.GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->SaveSceneNode3DXML(m_Editor.Get3DFrame().GetEditedRootNode3D());

	auto writer = manager.CreateWriter();
	writer->AddChild(rootWriter);

	auto file = manager.SaveWriterToFile(writer, fileName);
	file->Save();
}

void CEditorUIFrame::OnSceneClose()
{
	auto editorRoot = m_Editor.Get3DFrame().GetEditedRootNode3D();

	// Remove existing childs
	Log::Info("OnSceneClose: Root contains '%d' childs BEFORE load scene.", editorRoot->GetChilds().size());
	size_t mustLeaveExisting = 0;
	while (editorRoot->GetChilds().size() > mustLeaveExisting)
	{
		auto editorChild = *(editorRoot->GetChilds().begin() + mustLeaveExisting);
		if (editorChild->IsPersistance())
		{
			mustLeaveExisting++;
			Log::Info("OnSceneClose: Child '%s' was not removed, because is persistat", editorChild->GetName().c_str());
			continue;
		}

		editorRoot->RemoveChild(editorChild);
	}
	Log::Info("OnSceneClose: Root contains '%d' childs AFTER load scene.", editorRoot->GetChilds().size());
}
