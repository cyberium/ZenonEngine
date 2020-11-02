#include "stdafx.h"

// General
#include "EditorResourceBrowser.h"


#include "ZenonWidgets/ZenonTreeView/TreeViewItemVirtualFolder.h"



namespace
{
	class CznSceneNode3DTreeViewItemSource
		: public IznTreeViewItemSource
	{
	public:
		CznSceneNode3DTreeViewItemSource(const std::shared_ptr<ISceneNode3D>& SceneNode)
			: m_SceneNode(SceneNode)
		{}
		virtual ~CznSceneNode3DTreeViewItemSource()
		{
			Log::Warn("TreeViewSceneNode3DSource: Destroed source of object with name '%s'", m_SceneNode->GetName().c_str());
		}

		// IznTreeViewItemSource
		ETreeViewItemType GetType() const
		{
			return ETreeViewItemType::SceneNode3D;
		}
		std::string GetName() const override
		{
			return m_SceneNode->GetName();
		}
		size_t GetChildsCount() const
		{
			return m_SceneNode->GetChilds().size();
		}
		std::shared_ptr<IznTreeViewItemSource> GetChild(size_t Index) const
		{
			const auto& childs = m_SceneNode->GetChilds();
			//_ASSERT(Index < childs.size());
			if (Index >= childs.size())
				return nullptr;

			const auto& child = m_SceneNode->GetChilds().at(Index);
			return GetChildInternal(child);
		}
		std::shared_ptr<IObject> Object() const
		{
			return m_SceneNode;
		}

	private:
		std::shared_ptr<IznTreeViewItemSource> GetChildInternal(const std::shared_ptr<ISceneNode3D>& Object) const
		{
			if (Object == nullptr)
				return nullptr;

			const auto& it = std::find_if(m_CachedChilds.begin(), m_CachedChilds.end(), [Object](const std::shared_ptr<CznSceneNode3DTreeViewItemSource>& SourceObject)->bool
			{
				auto object = SourceObject->Object();
				if (object == nullptr)
					return false;
				return object == Object;
			});

			// Retrieve from cahce
			if (it != m_CachedChilds.end())
			{
				return *it;
			}

			// Add new item to cache
			auto newChild = MakeShared(CznSceneNode3DTreeViewItemSource, Object);
			Log::Info("TreeViewSceneNode3DSource: Created for object with name '%s'", Object->GetName().c_str());
			m_CachedChilds.push_back(newChild);
			return newChild;
		}

	private:
		std::shared_ptr<ISceneNode3D> m_SceneNode;

		mutable std::vector<std::shared_ptr<CznSceneNode3DTreeViewItemSource>> m_CachedChilds;
	};










	class CznModel3DTreeViewItemSource
		: public IznTreeViewItemSource
	{
	public:
		CznModel3DTreeViewItemSource(const std::shared_ptr<IModel>& Model)
			: m_Model(Model)
		{}

		// IznTreeViewItemSource
		ETreeViewItemType GetType() const
		{
			return ETreeViewItemType::Model;
		}
		std::string GetName() const override
		{
			return m_Model->GetName();
		}
		size_t GetChildsCount() const
		{
			return 0;
		}
		std::shared_ptr<IznTreeViewItemSource> GetChild(size_t Index) const
		{
			_ASSERT(false);
			return nullptr;
		}
		std::shared_ptr<IObject> Object() const
		{
			return m_Model;
		}
	private:
		std::shared_ptr<IModel> m_Model;
	};



}



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
	GetEditorQtUIFrame().getCollectionViewer()->SetOnSelectedItemChange([this](const CznTreeViewItem * Item) -> bool 
	{
		auto sourceObject = Item->GetSourceObject();
		if (sourceObject->GetType() != ETreeViewItemType::Model)
			return false;

		auto object = sourceObject->Object();
		if (object == nullptr)
			return false;

		auto modelObject = std::dynamic_pointer_cast<IModel>(object);
		if (modelObject == nullptr)
			return false;

		m_Editor.Get3DFrame().OnCollectionWidget_ModelSelected(modelObject);
		return true;
	});

	GetEditorQtUIFrame().getCollectionViewer()->SetOnStartDragging([this](const CznTreeViewItem * Item, std::string * Value) -> bool 
	{
		_ASSERT(Value != nullptr && Value->empty());
		auto sourceObject = Item->GetSourceObject();
		if (sourceObject->GetType() != ETreeViewItemType::Model)
			return false;
		auto object = sourceObject->Object();
		Value->assign(object->GetName().c_str());
		m_Editor.GetTools().Enable(ETool::EToolDragger);
		return true;
	});

	GetEditorQtUIFrame().getCollectionViewer()->AddToRoot(CreateModelsFromFolder("models"));
	GetEditorQtUIFrame().getCollectionViewer()->AddToRoot(CreateModelsFromFolder("models_td"));
}

std::shared_ptr<IznTreeViewItemSource> CEditorResourceBrowser::CreateModelsFromFolder(const std::string & FolderName)
{
	std::vector<std::shared_ptr<IznTreeViewItemSource>> models;
	auto gameDataStorage = GetBaseManager().GetManager<IFilesManager>()->GetStorage(EFilesStorageType::GAMEDATA);
	auto fileNames = gameDataStorage->GetAllFilesInFolder(FolderName, ".fbx");
	for (const auto& fbxFileName : fileNames)
	{
		try
		{
			auto filePtr = gameDataStorage->OpenFile(fbxFileName);
			filePtr->ChangeExtension("znmdl");

			if (GetBaseManager().GetManager<IFilesManager>()->IsFileExists(filePtr->Path_Name()))
			{
				IModelPtr model = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel(filePtr->Path_Name());
				model->SetName(filePtr->Name_NoExtension());
				models.push_back(MakeShared(CznModel3DTreeViewItemSource, model));
				continue;
			}

			CznFBXLoaderParams loader;
			if (fbxFileName.find("ground_dirt") != std::string::npos)
				loader.MakeCenterIsX0Z = true;
			if (fbxFileName.find("cliffGrey") != std::string::npos)
				loader.MakeCenterIsX0Z = true;
			if (fbxFileName.find("cliffBrown") != std::string::npos)
				loader.MakeCenterIsX0Z = true;

			loader.ApplyFullTransform = true;

			IModelPtr fbxModel = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel(fbxFileName, &loader);

			auto znModelFile = GetBaseManager().GetManager<IznModelsFactory>()->SaveModel(fbxModel, filePtr->Path_Name());
			znModelFile->Save();

			znModelFile->seek(0);

			auto znModel = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel(znModelFile);
			znModel->SetName(filePtr->Name_NoExtension());
			models.push_back(MakeShared(CznModel3DTreeViewItemSource, znModel));
		}
		catch (const CException& e)
		{
			Log::Error(e.MessageCStr());
		}
	}

	auto modelsFolders = MakeShared(CznTreeViewItemVirtualFolder, FolderName);
	for (const auto& m : models)
		modelsFolders->AddChild(m);
	return modelsFolders;
}




void CEditorResourceBrowser::InitializeSceneBrowser()
{
	GetEditorQtUIFrame().getSceneViewer()->SetOnSelectedItemChange([this](const CznTreeViewItem * Item) -> bool {
		auto sourceObject = Item->GetSourceObject();
		if (sourceObject->GetType() != ETreeViewItemType::SceneNode3D)
			return false;

		auto object = sourceObject->Object();
		if (object == nullptr)
			return false;

		auto sceneNode3DObject = std::dynamic_pointer_cast<ISceneNode3D>(object);
		if (sceneNode3DObject == nullptr)
			return false;

		m_Editor.Get3DFrame().LockUpdates();
		m_Editor.GetTools().GetToolT<IEditorToolSelector>(ETool::EToolSelector).SelectNode(sceneNode3DObject);
		m_Editor.Get3DFrame().UnlockUpdates();
		
		return true;
	});

	GetEditorQtUIFrame().getSceneViewer()->SetOnContexMenu([this](const CznTreeViewItem* Item, std::string * Title, std::vector<std::shared_ptr<IPropertyAction>> * Actions) -> bool {
		auto sourceObject = Item->GetSourceObject();
		if (sourceObject->GetType() != ETreeViewItemType::SceneNode3D)
			return false;

		auto object = sourceObject->Object();
		if (object == nullptr)
			return false;

		auto sceneNode3DObject = std::dynamic_pointer_cast<ISceneNode3D>(object);
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
		auto sceneNode3DSource = MakeShared(CznSceneNode3DTreeViewItemSource, m_Editor.Get3DFrame().GetEditedRootNode3D());
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
