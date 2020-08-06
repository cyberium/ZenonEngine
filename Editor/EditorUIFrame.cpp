#include "stdafx.h"

// General
#include "EditorUIFrame.h"

#include <qfilesystemmodel.h>

CEditorUIFrame::CEditorUIFrame(IEditor& Editor)
	: QMainWindow(nullptr)
	, m_Editor(Editor)
{
	dynamic_cast<IEditorPrivate&>(m_Editor).SetUIFrame(this);

	m_UI.setupUi(this);

	// MENU
	connect(m_UI.actionOpen_Scene, &QAction::triggered, [this]() {
		auto file = m_Editor.GetBaseManager().GetManager<IFilesManager>()->Open("D:\\Scene.xml");
		if (file == nullptr)
			return;

		CXMLManager xml;
		auto reader = xml.CreateReader(file);

		auto currentRoot = m_Editor.Get3DFrame().GetEditedRootNode3D();
		while (false == currentRoot->GetChilds().empty())
			currentRoot->RemoveChild(currentRoot->GetChilds()[0]);

		auto rootNodeXML = m_Editor.GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->LoadSceneNode3DXML(reader->GetChilds()[0], currentRoot->GetScene(), currentRoot->GetParent().lock());

		while (false == rootNodeXML->GetChilds().empty())
			currentRoot->AddChild(rootNodeXML->GetChilds()[0]);
	});
	connect(m_UI.actionSave_Scene, &QAction::triggered, [this]() {
		CXMLManager manager;

		auto rootWriter = m_Editor.GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->SaveSceneNode3DXML(m_Editor.Get3DFrame().GetEditedRootNode3D());

		auto writer = manager.CreateWriter();
		writer->AddChild(rootWriter);

		auto file = manager.SaveWriterToFile(writer, "D:\\Scene.xml");
		m_Editor.GetBaseManager().GetManager<IFilesManager>()->GetFilesStorage("PCEveryFileAccess")->SaveFile(file);
	});

	QFileSystemModel* fsModel = new QFileSystemModel(this);
	fsModel->setRootPath("D:\\_programming\\ZenonEngine\\gamedata");
	m_UI.FSTreeViewer->setModel(fsModel);

	m_PropertiesController = std::make_shared<CPropertiesController>(m_UI.PropertyEditor);

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

	auto fileNames = Utils::GetAllFilesInDirectory("C:\\_engine\\ZenonEngine_gamedata\\models", ".fbx");
	for (const auto& it : fileNames)
	{
		Log::Info(it.c_str());

		try
		{
			std::string zenonFileName = it + ".znmdl";
			if (m_Editor.GetBaseManager().GetManager<IFilesManager>()->IsFileExists(zenonFileName))
			{
				realNames.push_back(zenonFileName);
				continue;
			}

			continue;

			/*std::shared_ptr<ISceneNode3D> sceneNode = m_Editor.GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode_FBXNode, dynamic_cast<IScene*>(m_Editor3D));

			std::shared_ptr<IFBXNode> fbxSceneNode = std::dynamic_pointer_cast<IFBXNode>(sceneNode);
			fbxSceneNode->InitializeFromFile(it);

			auto model = std::dynamic_pointer_cast<IFBXNode>(sceneNode->GetChilds().at(0))->GetModel();
			if (auto loadable = std::dynamic_pointer_cast<IObjectLoadSave>(model))
			{
				std::shared_ptr<IFile> file = std::make_shared<CFile>(zenonFileName);
				loadable->Save(file);

				m_Editor.GetBaseManager().GetManager<IFilesManager>()->GetFilesStorage("PCEveryFileAccess")->SaveFile(file);
				realNames.push_back(zenonFileName);
			}*/
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

			std::shared_ptr<IPropertyAction> complexAction = std::make_shared<CAction>(actionName, "");
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

void CEditorUIFrame::OnSceneChanged()
{
	getSceneViewer()->RefreshTreeViewModel();
}



//
// IEditorQtUIFrame
//
QObject& CEditorUIFrame::getQObject()
{
	return *this;
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
