#include "stdafx.h"

// General
#include "EditorResourceBrowser.h"

#include "DragUtils.h"
#include "ResourcesBrowser/VirtualFolderTreeViewItemSource.h"
#include "ResourcesBrowser/Model3DTreeViewItemSource.h"
#include "ResourcesBrowser/NodeProtoTreeViewItemSource.h"
#include "ResourcesBrowser/SceneNodeTreeViewItemSource.h"



CEditorResourceBrowser::CEditorResourceBrowser(IEditor& Editor)
	: m_Editor(Editor)
	, IsAttachedTest(false)
{
}

CEditorResourceBrowser::~CEditorResourceBrowser()
{
}



//
// CEditorResourceBrowser
//
void CEditorResourceBrowser::Initialize()
{
	GetEditorQtUIFrame().getCollectionViewer()->SetOnSelectedItemChange([this](const IznTreeViewItem * Item) -> bool 
	{
		auto sourceObject = Item;
		auto object = sourceObject->GetObject_();
		if (object == nullptr)
			return false;

		if (sourceObject->GetType() == ETreeViewItemType::SceneNodeProto)
		{
			auto sceneNodeProto = std::dynamic_pointer_cast<ISceneNode>(object);
			if (sceneNodeProto == nullptr)
				return false;
			m_Editor.Get3DPreviewFrame().SetSceneNode(sceneNodeProto);
			return true;
		}
		else if (sourceObject->GetType() == ETreeViewItemType::Model)
		{
			auto modelObject = std::dynamic_pointer_cast<IModel>(object);
			if (modelObject == nullptr)
				return false;
			m_Editor.Get3DPreviewFrame().SetModel(modelObject);
			return true;
		}
		
		return false;
	});



	GetEditorQtUIFrame().getCollectionViewer()->SetOnStartDragging([this](const IznTreeViewItem * Item, CByteBuffer * Value) -> bool
	{
		if (Value == nullptr || Value->getSize() > 0 || Value->getPos() > 0)
		{
			Log::Error("Unable to start drag model event, because ByteBuffer is not empty.");
			return false;
		}

		auto sourceObject = Item;
		auto object = sourceObject->GetObject_();
		if (object == nullptr)
			return false;

		if (sourceObject->GetType() == ETreeViewItemType::SceneNodeProto)
		{
			auto objectAsModelAsSceneNodeProto = std::dynamic_pointer_cast<ISceneNode>(object);
			if (objectAsModelAsSceneNodeProto == nullptr)
				return false;

			CreateDragDataFromSceneNode(objectAsModelAsSceneNodeProto, Value);
			m_Editor.GetTools().Enable(ETool::EToolDragger);
			return true;
		}
		else if (sourceObject->GetType() == ETreeViewItemType::Model)
		{
			IModelPtr objectAsModel = std::dynamic_pointer_cast<IModel>(object);
			if (objectAsModel == nullptr)
				return false;

			CreateDragDataFromModel(objectAsModel, Value);
			m_Editor.GetTools().Enable(ETool::EToolDragger);
			return true;
		}

		return false;
	});

	GetEditorQtUIFrame().getCollectionViewer()->AddToRoot(CreateModelsFromFolder("models"));
	GetEditorQtUIFrame().getCollectionViewer()->AddToRoot(CreateModelsFromFolder("models_td"));
	GetEditorQtUIFrame().getCollectionViewer()->AddToRoot(CreateModelsFromFolder("models_food"));
	GetEditorQtUIFrame().getCollectionViewer()->AddToRoot(CreateSceneNodeProtosFromFolder("sceneNodesProtos"));
}

std::shared_ptr<IznTreeViewItem> CEditorResourceBrowser::CreateSceneNodeProtosFromFolder(const std::string & FolderName)
{
	std::vector<std::shared_ptr<IznTreeViewItem>> sceneNodes;

	auto gameDataStorage = GetBaseManager().GetManager<IFilesManager>()->GetStorage(EFilesStorageType::USERDATA);
	auto gameDataStorageEx = std::dynamic_pointer_cast<IznFilesStorageExtended>(gameDataStorage);
	_ASSERT(gameDataStorageEx != nullptr);

	auto fileNames = gameDataStorageEx->GetAllFilesInFolder(FolderName, ".xml");
	for (const auto& fileName : fileNames)
	{
		try
		{
			auto file = GetBaseManager().GetManager<IFilesManager>()->Open(fileName);

			CXMLManager xmlManager(GetBaseManager());
			auto reader = xmlManager.CreateReaderFromFile(file);
			_ASSERT(false == reader->GetChilds().empty());
			auto firstXMLChild = reader->GetChilds()[0];

			auto sceneNodeProtoRoot = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<CSceneNodeFactory>()->LoadSceneNode3DXML(firstXMLChild, m_Editor.Get3DFrame().GetScene());
			sceneNodeProtoRoot->MakeMeOrphan();
			sceneNodes.push_back(MakeShared(CzNodeProtoTreeViewItemSource, sceneNodeProtoRoot));
		}
		catch (const CException& e)
		{
			Log::Error("Error while loading '%s' SceneNodeProto.", fileName.c_str());
			Log::Error("--->%s", e.MessageCStr());
		}
	}

	auto modelsFolders = MakeShared(CznVirtualFolderTreeViewItemSource, FolderName);
	for (const auto& m : sceneNodes)
		modelsFolders->AddChild(m);
	return modelsFolders;
}

std::shared_ptr<IznTreeViewItem> CEditorResourceBrowser::CreateModelsFromFolder(const std::string & FolderName)
{
	std::vector<std::shared_ptr<IznTreeViewItem>> models;
	auto gameDataStorage = GetBaseManager().GetManager<IFilesManager>()->GetStorage(EFilesStorageType::USERDATA);
	auto gameDataStorageEx = std::dynamic_pointer_cast<IznFilesStorageExtended>(gameDataStorage);
	_ASSERT(gameDataStorageEx != nullptr);

	auto fileNames = gameDataStorageEx->GetAllFilesInFolder(FolderName, ".fbx");
	for (const auto& fbxFileName : fileNames)
	{
		auto t = MakeShared(CznModel3DTreeViewItemSource, GetBaseManager(), fbxFileName);
		m_Editor.GetBaseManager().GetManager<ILoader>()->AddToLoadQueue(t);
		//t->Load();
		//t->SetState(ILoadable::ELoadableState::Loaded);
		models.push_back(t);
	}

	auto modelsFolders = MakeShared(CznVirtualFolderTreeViewItemSource, FolderName);
	for (const auto& m : models)
		modelsFolders->AddChild(m);

	Log::Error("Resource Browser: All models from '%s' loaded.", FolderName.c_str());

	return modelsFolders;
}




void CEditorResourceBrowser::InitializeSceneBrowser()
{
	GetEditorQtUIFrame().getSceneViewer()->SetOnSelectedItemChange([this](const IznTreeViewItem * Item) -> bool {
		auto sourceObject = Item;
		if (sourceObject->GetType() != ETreeViewItemType::CSceneNode)
			return false;

		auto object = sourceObject->GetObject_();
		if (object == nullptr)
			return false;

		auto sceneNode3DObject = std::dynamic_pointer_cast<ISceneNode>(object);
		if (sceneNode3DObject == nullptr)
			return false;

		m_Editor.GetTools().GetToolT<IEditorToolSelector>(ETool::EToolSelector).SelectNode(sceneNode3DObject);
		return true;
	});

	GetEditorQtUIFrame().getSceneViewer()->SetOnContexMenu([this](const IznTreeViewItem* Item, std::string * Title, std::vector<std::shared_ptr<IPropertyAction>> * Actions) -> bool {
		auto sourceObject = Item;
		if (sourceObject->GetType() != ETreeViewItemType::CSceneNode)
			return false;

		auto object = sourceObject->GetObject_();
		if (object == nullptr)
			return false;

		auto sceneNode3DObject = std::dynamic_pointer_cast<ISceneNode>(object);
		if (sceneNode3DObject == nullptr)
			return false;
		
		if (false == GetEditorUIFrame().ExtendContextMenu(sceneNode3DObject, Title, Actions))
			return false;
		return true;
	});

	//UpdateSceneBrowser();
}






void CEditorResourceBrowser::UpdateSceneBrowser()
{
	if (false == IsAttachedTest)
	{
		auto sceneNode3DSource = MakeShared(CznSceneNode3DTreeViewItemSource, m_Editor.Get3DFrame().GetEditedRootNode3D(), nullptr);
		GetEditorQtUIFrame().getSceneViewer()->AddToRoot(sceneNode3DSource, true);
		IsAttachedTest = true;
	}
	else
	{
		GetEditorQtUIFrame().getSceneViewer()->Refresh();
	}
}



//
// Private
//
IBaseManager & CEditorResourceBrowser::GetBaseManager() const
{
	return m_Editor.GetBaseManager();
}

IEditorUIFrame & CEditorResourceBrowser::GetEditorUIFrame() const
{
	return m_Editor.GetUIFrame();
}

IEditorQtUIFrame& CEditorResourceBrowser::GetEditorQtUIFrame() const
{
	return dynamic_cast<IEditorQtUIFrame&>(m_Editor.GetUIFrame());
}
