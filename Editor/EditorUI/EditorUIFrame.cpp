#include "stdafx.h"

// General
#include "EditorUIFrame.h"

// Additional
#include "Frames/EditorTextureSelector.h"

CEditorUIFrame::CEditorUIFrame(IEditor& Editor)
	: QMainWindow(nullptr)
	, m_Editor(Editor)
	, m_EditorSceneBrowser(Editor)
	, m_EditorResourceBrowser(Editor)
	, m_FrameTexturesSelector(this, Editor)
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

	//m_PropertiesController = MakeShared(CPropertiesController, m_UI.PropertyEditor);

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
	m_EditorSceneBrowser.Initialize();
	m_EditorResourceBrowser.Initialize();

	return true;
}

void CEditorUIFrame::DoInitializeToolsUI()
{
	GetEditor().GetTools().DoInitializeUI(*this);

	getUI().editorTexturesTest->connect(getUI().editorTexturesTest, &QPushButton::released, [this]() {
		m_FrameTexturesSelector.show();
	});
}

bool CEditorUIFrame::ExtendContextMenu(const std::shared_ptr<ISceneNode>& Node, std::shared_ptr<IPropertiesGroup> PropertiesGroup)
{
	if (false == GetEditor().IsNodeSelected(Node))
		return false;

	std::string title = Node->GetName();
	

	if (GetEditor().GetSelectedNodes().size() > 1)
	{
		title.append("_and_" + std::to_string(GetEditor().GetSelectedNodes().size()) + "_nodes");

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

			PropertiesGroup->AddProperty(complexAction);
		}
	}
	else
	{
		auto newPropertiesGroup = MakeShared(CPropertiesGroup, "New", "Create some new elements.");
		PropertiesGroup->AddProperty(newPropertiesGroup);

		auto newSceneNode = MakeShared(CAction, "SceneNode", "...");
		newSceneNode->SetAction([this, Node]() -> bool {
			auto newCreatedNode = GetEditor().Get3DFrame().GetEditedScene()->CreateSceneNodeT<ISceneNode>(Node);
			newCreatedNode->SetName("SceneNode");

			return true;
		});
		newPropertiesGroup->AddProperty(newSceneNode);

		for (const auto& it : Node->GetProperties()->GetProperties())
		{
			if (auto act = std::dynamic_pointer_cast<IPropertyAction>(it.second))
			{
				PropertiesGroup->AddProperty(act);
			}
		}
	}

	PropertiesGroup->SetName(title);

	return true;
}

glm::vec3 CEditorUIFrame::FixMoverCoords(const glm::vec3 & Position) const
{
	glm::vec3 newPosition = Position;
	newPosition /= m_MoverValue;
	newPosition = glm::round(newPosition);
	newPosition *= m_MoverValue;
	return newPosition;
}

void CEditorUIFrame::SetMoverValue(float Value)
{
	m_MoverValue = Value;
}

float CEditorUIFrame::GetMoverValue() const
{
	return m_MoverValue;
}

float CEditorUIFrame::FixRotatorCoords(float Angle) const
{
	float rotatorInitialAngleDegrees = Angle;
	rotatorInitialAngleDegrees /= m_RotatorValue;
	rotatorInitialAngleDegrees = glm::round(rotatorInitialAngleDegrees);
	rotatorInitialAngleDegrees *= m_RotatorValue;
	return rotatorInitialAngleDegrees;
}

void CEditorUIFrame::SetRotatorValue(float Value)
{
	m_RotatorValue = Value;
}

float CEditorUIFrame::GetRotatorValue() const
{
	return m_RotatorValue;
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
	else if (selectedNodes.size() == 1)
	{
		auto firstSelectedNode = GetEditor().GetFirstSelectedNode();
		getSceneViewer()->SelectItem(firstSelectedNode);
		GetEditor().GetTools().OnNodeSelected(firstSelectedNode);

		getUI().NewPropsWidget->SetProperties(firstSelectedNode->GetProperties());
	}
	else
	{
		getSceneViewer()->ClearSelection();
		for (const auto& selectedNode : selectedNodes)
			getSceneViewer()->SelectItem(selectedNode, false);
	}
}




//
// ISceneEventsListener
//
void CEditorUIFrame::OnSceneNodeAdded(std::shared_ptr<ISceneNode> ParentNode, std::shared_ptr<ISceneNode> ChildNode)
{
	if (IsChildOf(GetEditor().Get3DFrame().GetEditedRootNode3D(), ChildNode) || IsChildOf(GetEditor().Get3DFrame().GetEditedRootNode3D(), ParentNode))
	{
		m_EditorSceneBrowser.Update();
	}
}

void CEditorUIFrame::OnSceneNodeRemoved(std::shared_ptr<ISceneNode> ParentNode, std::shared_ptr<ISceneNode> ChildNode)
{
	if (IsChildOf(GetEditor().Get3DFrame().GetEditedRootNode3D(), ChildNode) || IsChildOf(GetEditor().Get3DFrame().GetEditedRootNode3D(), ParentNode))
	{
		m_Editor.GetTools().GetToolT<IEditorToolSelector>(ETool::EToolSelector).RemoveNode(ChildNode);
		m_EditorSceneBrowser.Update();
	}
}




//
// Protected
//
void CEditorUIFrame::OnSceneLoadFromFile()
{
	try
	{
		std::string fileName = GetEditor().GetShell().ShowLoadFileDialog("");
		if (fileName.empty())
			throw CException("Filename is empty.");

		auto sceneLoadSave = std::dynamic_pointer_cast<ISceneLoadSave>(GetEditor().Get3DFrame().GetEditedScene());
		if (sceneLoadSave == nullptr)
			throw CException("Edited scene don't supports 'ISceneLoadSave'.");

		sceneLoadSave->LoadFromFile(fileName);

		m_EditorSceneBrowser.Update();
	}
	catch (const CException& e)
	{
		GetEditor().GetShell().ShowErrorDialog("Error while load scene from file.\r\n%s", e.MessageCStr());
	}
}

void CEditorUIFrame::OnSceneSaveToFile()
{
	try
	{
		std::string fileName = GetEditor().GetShell().ShowSaveFileDialog("");
		if (fileName.empty())
			throw CException("Filename is empty.");

		auto sceneLoadSave = std::dynamic_pointer_cast<ISceneLoadSave>(GetEditor().Get3DFrame().GetEditedScene());
		if (sceneLoadSave == nullptr)
			throw CException("Edited scene don't supports 'ISceneLoadSave'.");

		sceneLoadSave->SaveToFile(fileName);

		m_EditorSceneBrowser.Update();
	}
	catch (const CException& e)
	{
		GetEditor().GetShell().ShowErrorDialog("Error while save scene to file.\r\n%s", e.MessageCStr());
	}
}

void CEditorUIFrame::OnSceneClose()
{
	try
	{
		auto sceneLoadSave = std::dynamic_pointer_cast<ISceneLoadSave>(GetEditor().Get3DFrame().GetEditedScene());
		if (sceneLoadSave == nullptr)
			throw CException("Edited scene don't supports 'ISceneLoadSave'.");

		sceneLoadSave->ResetScene();

		m_EditorSceneBrowser.Update();
	}
	catch (const CException& e)
	{
		GetEditor().GetShell().ShowErrorDialog("Error while close scene.\r\n%s", e.MessageCStr());
	}
}
