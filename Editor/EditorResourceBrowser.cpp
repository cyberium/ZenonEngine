#include "stdafx.h"

// General
#include "EditorResourceBrowser.h"






namespace
{
	class CznSceneNode3DTreeViewItemSource
		: public IznTreeViewItemSource
	{
	public:
		CznSceneNode3DTreeViewItemSource(const std::shared_ptr<ISceneNode3D>& SceneNode)
			: m_SceneNode(SceneNode)
		{}

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
			_ASSERT(Index < childs.size());
			const auto& child = m_SceneNode->GetChilds().at(Index);
			return MakeShared(CznSceneNode3DTreeViewItemSource, child);
		}
		std::shared_ptr<IObject> Object() const
		{
			return m_SceneNode;
		}

	private:
		std::shared_ptr<ISceneNode3D> m_SceneNode;
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
		//const std::vector<std::shared_ptr<IznTreeViewItemSource>>& GetChilds() override
		//{
		//	return m_Childs;
		//}
		std::shared_ptr<IObject> Object() const
		{
			return m_Model;
		}
	private:
		std::shared_ptr<IModel> m_Model;
		//std::vector<std::shared_ptr<IznTreeViewItemSource>> m_Childs;
	};



}



CEditorResourceBrowser::CEditorResourceBrowser(IEditor& Editor)
	: m_Editor(Editor)
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
	std::vector<std::shared_ptr<IznTreeViewItemSource>> models;
	auto gameDataStorage = GetBaseManager().GetManager<IFilesManager>()->GetStorage(EFilesStorageType::GAMEDATA);
	auto fileNames = gameDataStorage->GetAllFilesInFolder("models", ".fbx");
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
			if (fbxFileName.find_first_of("ground_dirt") != std::string::npos)
				loader.MakeCenterIsX0Z = true;
			if (fbxFileName.find_first_of("cliffGrey") != std::string::npos)
				loader.MakeCenterIsX0Z = true;
			if (fbxFileName.find_first_of("cliffBrown") != std::string::npos)
				loader.MakeCenterIsX0Z = true;

			auto fbxModel = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel(fbxFileName, &loader);
			auto znModelFile = GetBaseManager().GetManager<IznModelsFactory>()->SaveModel(fbxModel, filePtr->Path_Name());
			znModelFile->Save();

			models.push_back(MakeShared(CznModel3DTreeViewItemSource, GetBaseManager().GetManager<IznModelsFactory>()->LoadModel(znModelFile)));
		}
		catch (const CException& e)
		{
			Log::Error(e.MessageCStr());
		}
	}

	GetEditorQtUIFrame().getCollectionViewer()->AddToRoot(models);

	GetEditorQtUIFrame().getCollectionViewer()->SetOnSelectedItemChange([this](const CznTreeViewItem * Item) -> bool 
	{
		m_Editor.Get3DFrame().OnCollectionWidget_ModelSelected(std::dynamic_pointer_cast<IModel>(Item->GetTObject()));
		return true;
	});

	GetEditorQtUIFrame().getCollectionViewer()->SetOnStartDragging([this](const CznTreeViewItem * Item, std::string * Value) -> bool 
	{
		_ASSERT(Value != nullptr && Value->empty());
		Value->assign(Item->GetTObject()->GetName().c_str());
		m_Editor.GetTools().Enable(ETool::EToolDragger);
		return true;
	});
}




void CEditorResourceBrowser::InitializeSceneBrowser()
{
	GetEditorQtUIFrame().getSceneViewer()->SetOnSelectedItemChange([this](const CznTreeViewItem * Item) -> bool {
		m_Editor.Get3DFrame().LockUpdates();
		{
			auto& selector = dynamic_cast<IEditorToolSelector&>(m_Editor.GetTools().GetTool(ETool::EToolSelector));
			selector.SelectNode(std::static_pointer_cast<ISceneNode3D>(Item->GetTObject()));
		}
		m_Editor.Get3DFrame().UnlockUpdates();
		return true;
	});

	GetEditorQtUIFrame().getSceneViewer()->SetOnContexMenu([this](const CznTreeViewItem* Item, std::string * Title, std::vector<std::shared_ptr<IPropertyAction>> * Actions) -> bool {
		if (false == GetEditorUIFrame().ExtendContextMenu(std::dynamic_pointer_cast<ISceneNode3D>(Item->GetTObject()), Title, Actions))
			return false;
		return true;
	});

	//UpdateSceneBrowser();
}

void CEditorResourceBrowser::UpdateSceneBrowser()
{
	auto sceneNode3DSource = MakeShared(CznSceneNode3DTreeViewItemSource, m_Editor.Get3DFrame().GetEditedRootNode3D());
	GetEditorQtUIFrame().getSceneViewer()->AddToRoot(sceneNode3DSource, true);
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
