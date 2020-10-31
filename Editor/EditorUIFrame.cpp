#include "stdafx.h"

// General
#include "EditorUIFrame.h"


namespace
{
	class CSceneNodeModelItem
		: public IModelCollectionItem
	{
	public:
		CSceneNodeModelItem(const std::shared_ptr<ISceneNode3D>& SceneNode)
			: m_SceneNode(SceneNode)
		{
			for (const auto& it : SceneNode->GetChilds())
				m_Childs.push_back(MakeShared(CSceneNodeModelItem, it));
		}

		std::string GetName() const override
		{
			return m_SceneNode->GetName();
		}
		const std::vector<std::shared_ptr<IModelCollectionItem>>& GetChilds() override
		{
			return m_Childs;
		}
		std::shared_ptr<IObject> Object() const
		{
			return m_SceneNode;
		}

	private:
		std::shared_ptr<ISceneNode3D> m_SceneNode;
		std::vector<std::shared_ptr<IModelCollectionItem>> m_Childs;
	};


	class C3DModelModelItem
		: public IModelCollectionItem
	{
	public:
		C3DModelModelItem(const std::shared_ptr<IModel>& Model)
			: m_Model(Model)
		{
		}

		std::string GetName() const override
		{
			return m_Model->GetName();
		}
		const std::vector<std::shared_ptr<IModelCollectionItem>>& GetChilds() override
		{
			return m_Childs;
		}
		std::shared_ptr<IObject> Object() const
		{
			return m_Model;
		}

	private:
		std::shared_ptr<IModel> m_Model;
		std::vector<std::shared_ptr<IModelCollectionItem>> m_Childs;
	};
}



CEditorUIFrame::CEditorUIFrame(IEditor& Editor)
	: QMainWindow(nullptr)
	, m_Editor(Editor)
{
	dynamic_cast<IEditorPrivate&>(m_Editor).SetUIFrame(this);

	m_UI.setupUi(this);

	// Load & Save
	connect(m_UI.actionOpen_Scene, &QAction::triggered, std::bind(&CEditorUIFrame::OnSceneLoadFromFile, this));
	connect(m_UI.actionSave_Scene, &QAction::triggered, std::bind(&CEditorUIFrame::OnSceneSaveToFile, this));

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



	// Models viewer
#pragma region Models viewer
	std::vector<std::shared_ptr<IModelCollectionItem>> models;
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
				IModelPtr model = m_Editor.GetBaseManager().GetManager<IznModelsFactory>()->LoadModel(filePtr->Path_Name());
				model->SetName(filePtr->Name_NoExtension());
				models.push_back(MakeShared(C3DModelModelItem, model));
				continue;
			}

			CznFBXLoaderParams loader;
			if (fbxFileName.find_first_of("ground_dirt") != std::string::npos)
				loader.MakeCenterIsX0Z = true;
			if (fbxFileName.find_first_of("cliffGrey") != std::string::npos)
				loader.MakeCenterIsX0Z = true;
			if (fbxFileName.find_first_of("cliffBrown") != std::string::npos)
				loader.MakeCenterIsX0Z = true;

			auto fbxModel = m_Editor.GetBaseManager().GetManager<IznModelsFactory>()->LoadModel(fbxFileName, &loader);
			auto znModelFile = m_Editor.GetBaseManager().GetManager<IznModelsFactory>()->SaveModel(fbxModel, filePtr->Path_Name());
			znModelFile->Save();

			models.push_back(MakeShared(C3DModelModelItem, m_Editor.GetBaseManager().GetManager<IznModelsFactory>()->LoadModel(znModelFile)));
		}
		catch (const CException& e)
		{
			Log::Error(e.MessageCStr());
		}
	}

	getCollectionViewer()->SetRootItems(models);

	getCollectionViewer()->SetOnSelectedItemChange([this](const CQtToZenonTreeItem * Item) -> bool {
		m_Editor.Get3DFrame().OnCollectionWidget_ModelSelected(std::dynamic_pointer_cast<IModel>(Item->GetTObject()));
		return true;
	});

	getCollectionViewer()->SetOnStartDragging([this](const CQtToZenonTreeItem * Item, std::string * Value) -> bool {
		_ASSERT(Value != nullptr && Value->empty());
		Value->assign(Item->GetTObject()->GetName().c_str());
		m_Editor.GetTools().Enable(ETool::EToolDragger);
		return true;
	});
#pragma endregion



	// SceneNode viewer
#pragma region SceneNode viewer
	getSceneViewer()->SetRootItems(models);

	getSceneViewer()->SetOnSelectedItemChange([this](const CQtToZenonTreeItem * Item) -> bool {
		m_Editor.Get3DFrame().LockUpdates();
		auto& selector = dynamic_cast<IEditorToolSelector&>(m_Editor.GetTools().GetTool(ETool::EToolSelector));
		selector.SelectNode(std::static_pointer_cast<ISceneNode3D>(Item->GetTObject()));
		m_Editor.Get3DFrame().UnlockUpdates();
		return true;
	});

	getSceneViewer()->SetOnContexMenu([this](const CQtToZenonTreeItem* Item, std::string * Title, std::vector<std::shared_ptr<IPropertyAction>> * Actions) -> bool {
		if (false == m_Editor.GetUIFrame().ExtendContextMenu(std::dynamic_pointer_cast<ISceneNode3D>(Item->GetTObject()), Title, Actions))
			return false;
		return true;
	});
#pragma endregion


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
	getSceneViewer()->SetRootItem(MakeShared(CSceneNodeModelItem, m_Editor.Get3DFrame().GetEditedRootNode3D()));
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

	//m_PropertiesController->OnSceneNodeSelected(GetEditor().GetFirstSelectedNode().get());
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

	CXMLManager xml(m_Editor.GetBaseManager());
	auto reader = xml.CreateReader(file);

	auto editorRoot = m_Editor.Get3DFrame().GetEditedRootNode3D();

	// Remove existing childs
	Log::Info("SceneLoad: Root contains '%d' childs BEFORE load scene.", editorRoot->GetChilds().size());
	size_t mustLeaveExisting = 0;
	while (editorRoot->GetChilds().size() > mustLeaveExisting)
	{
		auto editorChild = *(editorRoot->GetChilds().begin() + mustLeaveExisting);
		if (editorChild->IsPersistance())
		{
			mustLeaveExisting++;
			Log::Info("SceneLoad: Child '%s' was not removed, because is persistat", editorChild->GetName().c_str());
			continue;
		}

		editorRoot->RemoveChild(editorChild);
	}
	Log::Info("SceneLoad: Root contains '%d' childs AFTER load scene.", editorRoot->GetChilds().size());



	auto realRoot = editorRoot->GetParent().lock(); // Temporary we add nodes to real root
	auto xmlRoot = m_Editor.GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->LoadSceneNode3DXML(reader->GetChilds()[0], editorRoot->GetScene(), realRoot);



	// Update persistance nodes
	for (const auto& editorChild : editorRoot->GetChilds())
	{
		if (editorChild->IsPersistance())
		{
			auto xmlChild = xmlRoot->GetChild(editorChild->GetName());
			_ASSERT(xmlChild != nullptr);
			xmlChild->CopyTo(editorChild);
			xmlRoot->RemoveChild(xmlChild);
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
