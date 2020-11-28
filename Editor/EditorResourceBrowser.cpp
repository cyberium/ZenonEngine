#include "stdafx.h"

// General
#include "EditorResourceBrowser.h"

#include "DragUtils.h"
#include "ResourcesBrowser/FolderTreeViewItem.h"
#include "ResourcesBrowser/ModelTreeViewItem.h"
#include "ResourcesBrowser/NodeProtoTreeViewItem.h"
#include "ResourcesBrowser/SceneNodeTreeViewItem.h"

#include "ResourcesBrowser/ResourcesFilesystem.h"


/*
  Engine formats:
    Models:
	  *.dds - Texture
	  *.png - Texture

	  *.znxobj - xml ZenonEngine object

	  *.znxscn - xml ZenonEngine scene

	  *.znxmat - xml material
	
	  *.znxgtr - xml geometry

	  *.fbx    - model

	  *.znmdl  - binary model
	  *.znxmdl - xml model
*/

namespace
{
	std::shared_ptr<IznTreeViewItem> ResourceFileToTreeView(IEditor& Editor, std::shared_ptr<IResourceFile> ResourceFile)
	{
		if (ResourceFile->IsDirectory())
		{
			auto directoryTreeViewItem = MakeShared(CFolderTreeViewItem, ResourceFile->GetFilenameStruct().NameWithoutExtension);

			for (const auto& childResourceFile : ResourceFile->GetChilds())
			{
				auto treeViewItem = ResourceFileToTreeView(Editor, childResourceFile);
				if (treeViewItem != nullptr)
					directoryTreeViewItem->AddChild(treeViewItem);
			}

			return directoryTreeViewItem;
		}
		else if (ResourceFile->IsFile())
		{
			_ASSERT(ResourceFile->GetChilds().empty());

			const auto& fileNameStruct = ResourceFile->GetFilenameStruct();
			if (fileNameStruct.Extension == "znmdl" || fileNameStruct.Extension == "znxmdl")
			{
				auto modelTreeViewItem = MakeShared(CModelTreeViewItem, Editor.GetBaseManager(), fileNameStruct.ToString());
				Editor.GetBaseManager().GetManager<ILoader>()->AddToLoadQueue(modelTreeViewItem);

				//t->Load();
				//t->SetState(ILoadable::ELoadableState::Loaded);

				return modelTreeViewItem;
			}
			else if (fileNameStruct.Extension == "znobj")
			{
				try
				{
					CXMLManager xmlManager(Editor.GetBaseManager());
					auto reader = xmlManager.CreateReaderFromFile(fileNameStruct.ToString());
					_ASSERT(false == reader->GetChilds().empty());
					auto firstXMLChild = reader->GetChilds()[0];

					auto sceneNodeProtoRoot = Editor.GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<CSceneNodeFactory>()->LoadSceneNode3DXML(firstXMLChild, Editor.Get3DFrame().GetScene());
					sceneNodeProtoRoot->MakeMeOrphan();

					auto nodeProtoTreeViewItem = MakeShared(CNodeProtoTreeViewItem, sceneNodeProtoRoot);

					return nodeProtoTreeViewItem;
				}
				catch (const CException& e)
				{
					Log::Error("Error while loading '%s' NodeProto.", fileNameStruct.ToString().c_str());
					Log::Error("--->%s", e.MessageCStr());
				}
			}
			else
				Log::Warn("Resource file '%s' has unsupported format.", fileNameStruct.ToString().c_str());
		}

		return nullptr;
	}
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

	CResourceFilesystem fileSystem;
	fileSystem.Initailize("O:/ZenonEngine_userdata");
	fileSystem.PrintFilesystem();

	GetEditorQtUIFrame().getCollectionViewer()->Refresh();
	for (const auto& resourceFile : fileSystem.GetRootFile()->GetChilds())
	{
		auto treeViewItem = ResourceFileToTreeView(m_Editor, resourceFile);
		if (treeViewItem != nullptr)
			GetEditorQtUIFrame().getCollectionViewer()->AddToRoot(treeViewItem);
	}
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
		auto sceneNode3DSource = MakeShared(CSceneNodeTreeViewItem, m_Editor.Get3DFrame().GetEditedRootNode3D(), nullptr);
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
