#include "stdafx.h"

// General
#include "EditorResourceBrowser.h"

// Additional
#include "DragUtils.h"

#include "TreeViewItems/FolderTreeViewItem.h"
#include "TreeViewItems/ModelTreeViewItem.h"
#include "TreeViewItems/NodeProtoTreeViewItem.h"
#include "TreeViewItems/TextureTreeViewItem.h"

#include "Filesystem/ResourcesFilesystem.h"


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
				if (auto treeViewItem = ResourceFileToTreeView(Editor, childResourceFile))
					directoryTreeViewItem->AddChild(treeViewItem);

			// Skip empty directories
			if (directoryTreeViewItem->GetChildsCount() == 0)
				return nullptr;

			return directoryTreeViewItem;
		}
		else if (ResourceFile->IsFile())
		{
			_ASSERT(ResourceFile->GetChilds().empty());

			const auto& fileNameStruct = ResourceFile->GetFilenameStruct();
			if (/*fileNameStruct.Extension == "znmdl" || */fileNameStruct.Extension == "znxmdl" /*|| fileNameStruct.Extension == "fbx"*/)
			{
				auto modelTreeViewItem = MakeShared(CModelTreeViewItem, Editor.GetBaseManager(), fileNameStruct.ToString());
				
				//Editor.GetBaseManager().GetManager<ILoader>()->AddToLoadQueue(modelTreeViewItem);

				modelTreeViewItem->Load();
				modelTreeViewItem->SetState(ILoadable::ELoadableState::Loaded);

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

					return MakeShared(CNodeProtoTreeViewItem, sceneNodeProtoRoot);
				}
				catch (const CException& e)
				{
					Log::Error("Error while loading '%s' NodeProto.", fileNameStruct.ToString().c_str());
					Log::Error("--->%s", e.MessageCStr());
				}
			}
			else if (/*fileNameStruct.Extension == "dds"  ||*/ fileNameStruct.Extension == "png")
			{
				auto texture = Editor.GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D(fileNameStruct.ToString());
				return MakeShared(CTextureTreeViewItem, texture);
			}
			else
				Log::Warn("Resource file '%s' has unsupported format.", fileNameStruct.ToString().c_str());
		}

		return nullptr;
	}
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
	GetEditorQtUIFrame().getCollectionViewer()->SetOnSelectedItemChange(std::bind(&CEditorResourceBrowser::OnSelectTreeItem, this, std::placeholders::_1));
	GetEditorQtUIFrame().getCollectionViewer()->SetOnStartDragging(std::bind(&CEditorResourceBrowser::OnStartDraggingTreeItem, this, std::placeholders::_1, std::placeholders::_2));

	CResourceFilesystem fileSystem;
	fileSystem.Initailize("O:/ZenonEngine_userdata");
	fileSystem.PrintFilesystem();

	GetEditorQtUIFrame().getCollectionViewer()->Refresh();
	for (const auto& resourceFile : fileSystem.GetRootFile()->GetChilds())
		if (auto treeViewItem = ResourceFileToTreeView(m_Editor, resourceFile))
			GetEditorQtUIFrame().getCollectionViewer()->AddToRoot(treeViewItem);
}



//
// Events
//
bool CEditorResourceBrowser::OnSelectTreeItem(const IznTreeViewItem * Item)
{
	auto object = Item->GetObject_();
	if (object == nullptr)
		return false;

	if (Item->GetType() == ETreeViewItemType::SceneNodeProto)
	{
		auto sceneNodeProto = std::dynamic_pointer_cast<ISceneNode>(object);
		if (sceneNodeProto == nullptr)
		{
			Log::Error("Editor: TreeView item has type 'SceneNodeProto', but object don't have.");
			return false;
		}

		m_Editor.Get3DPreviewFrame().SetSceneNode(sceneNodeProto);
		return true;
	}
	else if (Item->GetType() == ETreeViewItemType::Model)
	{
		auto modelObject = std::dynamic_pointer_cast<IModel>(object);
		if (modelObject == nullptr)
		{
			Log::Error("Editor: TreeView item has type 'Model', but object don't have.");
			return false;
		}

		m_Editor.Get3DPreviewFrame().SetModel(modelObject);
		return true;
	}
	else if (Item->GetType() == ETreeViewItemType::Texture)
	{
		auto textureObject = std::dynamic_pointer_cast<ITexture>(object);
		if (textureObject == nullptr)
		{
			Log::Error("Editor: TreeView item has type 'Texture', but object don't have.");
			return false;
		}

		m_Editor.Get3DPreviewFrame().SetTexture(textureObject);
		return true;
	}

	return false;
}

bool CEditorResourceBrowser::OnStartDraggingTreeItem(const IznTreeViewItem * Item, CByteBuffer * Value)
{
	if (Value == nullptr || Value->getSize() > 0 || Value->getPos() > 0)
	{
		Log::Error("Unable to start drag model event, because ByteBuffer is not empty.");
		return false;
	}

	auto object = Item->GetObject_();
	if (object == nullptr)
		return false;

	//
	// Node proto
	//
	if (Item->GetType() == ETreeViewItemType::SceneNodeProto)
	{
		auto objectAsModelAsSceneNodeProto = std::dynamic_pointer_cast<ISceneNode>(object);
		if (objectAsModelAsSceneNodeProto == nullptr)
			return false;

		CreateDragDataFromSceneNode(objectAsModelAsSceneNodeProto, Value);
		m_Editor.GetTools().Enable(ETool::EToolDragger);
		return true;
	}


	//
	// Model
	//
	else if (Item->GetType() == ETreeViewItemType::Model)
	{
		std::shared_ptr<IModel> objectAsModel = std::dynamic_pointer_cast<IModel>(object);
		if (objectAsModel == nullptr)
			return false;

		CreateDragDataFromModel(objectAsModel, Value);
		m_Editor.GetTools().Enable(ETool::EToolDragger);
		return true;
	}

	//
	// Texture
	//
	else if (Item->GetType() == ETreeViewItemType::Texture)
	{
		std::shared_ptr<ITexture> objectAsTexture = std::dynamic_pointer_cast<ITexture>(object);
		if (objectAsTexture == nullptr)
			return false;

		CreateDragDataFromTexture(objectAsTexture, Value);
		m_Editor.GetTools().Enable(ETool::EToolDragger);
		return true;
	}

	return false;
}

bool CEditorResourceBrowser::OnContextMenuTreeItem(const IznTreeViewItem * Item, std::string * ContextMenuTitle, std::vector<std::shared_ptr<IPropertyAction>> * ResultActions)
{
	if (Item->GetType() != ETreeViewItemType::VirtualFolder)
		return false;

	*ContextMenuTitle = "Create";

	/*auto removeAction = MakeShared(CAction, "Create New particle", "");
	removeAction->SetAction([this]() -> bool {
		GetParent()->RemoveChild(shared_from_this());
		return true;
	});

	removeAction->SetActionPrecondition([this]() -> bool {
		return false == IsPersistance();
	});

	ResultActions->push_back();
	*/
	return false;
}


bool CEditorResourceBrowser::CreateNewParticle() const
{
	return false;
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
