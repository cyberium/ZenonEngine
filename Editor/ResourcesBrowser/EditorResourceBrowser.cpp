#include "stdafx.h"

// General
#include "EditorResourceBrowser.h"

// Additional
#include "EditorUI/DragUtils.h"

#include "TreeViewItems/FolderTreeViewItem.h"
#include "TreeViewItems/ModelTreeViewItem.h"
#include "TreeViewItems/NodeProtoTreeViewItem.h"
#include "TreeViewItems/TextureTreeViewItem.h"
#include "TreeViewItems/ParticleSystemTreeViewItem.h"
#include "TreeViewItems/MaterialTreeViewItem.h"

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
			if (/*fileNameStruct.Extension == "znmdl" ||*/ fileNameStruct.Extension == "znxmdl" /*|| fileNameStruct.Extension == "fbx"*/)
			{
				auto modelTreeViewItem = MakeShared(CModelTreeViewItem, Editor.GetBaseManager(), fileNameStruct.ToString());
				
				Editor.GetBaseManager().GetManager<ILoader>()->AddToLoadQueue(modelTreeViewItem);

				//modelTreeViewItem->Load();
				//modelTreeViewItem->SetState(ILoadable::ELoadableState::Loaded);

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
				auto textureTreeViewItem = MakeShared(CTextureTreeViewItem, Editor.GetBaseManager(), fileNameStruct.ToString());
				
				Editor.GetBaseManager().GetManager<ILoader>()->AddToLoadQueue(textureTreeViewItem);
				
				return textureTreeViewItem;
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
	GetEditorQtUIFrame().getResourcesBrowser()->SetOnSelectedItemChange(std::bind(&CEditorResourceBrowser::OnSelectTreeItem, this, std::placeholders::_1));
	GetEditorQtUIFrame().getResourcesBrowser()->SetOnStartDragging(std::bind(&CEditorResourceBrowser::OnStartDraggingTreeItem, this, std::placeholders::_1, std::placeholders::_2));
	GetEditorQtUIFrame().getResourcesBrowser()->SetOnContexMenu(std::bind(&CEditorResourceBrowser::OnContextMenuTreeItem, this, std::placeholders::_1, std::placeholders::_2));
	GetEditorQtUIFrame().getResourcesBrowser()->setSelectionMode(QAbstractItemView::SingleSelection);
	
	CResourceFilesystem fileSystem;
	fileSystem.Initailize("D:/ZenonEngine_userdata");
	//fileSystem.PrintFilesystem();

	GetEditorQtUIFrame().getResourcesBrowser()->Refresh();
	for (const auto& resourceFile : fileSystem.GetRootFile()->GetChilds())
		if (auto treeViewItem = ResourceFileToTreeView(GetEditor(), resourceFile))
			GetEditorQtUIFrame().getResourcesBrowser()->AddToRoot(treeViewItem);
			
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

		GetEditor().Get3DPreviewFrame().SetSceneNode(sceneNodeProto);
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

		GetEditor().Get3DPreviewFrame().SetModel(modelObject);
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

		GetEditor().Get3DPreviewFrame().SetTexture(textureObject);
		return true;
	}
	else if (Item->GetType() == ETreeViewItemType::ParticleSystem)
	{
		auto particleSystemObject = std::dynamic_pointer_cast<IParticleSystem>(object);
		if (particleSystemObject == nullptr)
		{
			Log::Error("Editor: TreeView item has type 'ParticleSystem', but object don't have.");
			return false;
		}
		
		GetEditorQtUIFrame().getUI().NewPropsWidget->SetProperties(particleSystemObject->GetProperties());
		GetEditor().Get3DPreviewFrame().SetParticleSystem(particleSystemObject);
		GetEditorQtUIFrame().getResourcesBrowser()->SelectItem(particleSystemObject);

		return true;
	}
	else if (Item->GetType() == ETreeViewItemType::Material)
	{
		auto materialObject = std::dynamic_pointer_cast<IMaterial>(object);
		if (materialObject == nullptr)
		{
			Log::Error("Editor: TreeView item has type 'Material', but object don't have.");
			return false;
		}

		GetEditorQtUIFrame().getUI().NewPropsWidget->SetProperties(materialObject->GetProperties());
		GetEditor().Get3DPreviewFrame().SetMaterial(materialObject);
		GetEditorQtUIFrame().getResourcesBrowser()->SelectItem(materialObject);

		return true;
	}

	return false;
}

bool CEditorResourceBrowser::OnStartDraggingTreeItem(const IznTreeViewItem * Item, CByteBuffer * Value)
{
	if (Value == nullptr || Value->getSize() > 0 || Value->getPos() > 0)
		throw CException("Unable to 'OnStartDraggingTreeItem', because ByteBuffer is not empty.");

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

		CreateDragDataFromSceneNodeProto(objectAsModelAsSceneNodeProto, Value);
		//GetEditor().GetTools().Enable(ETool::EToolDragger);
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
		//GetEditor().GetTools().Enable(ETool::EToolDragger);
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
		//GetEditor().GetTools().Enable(ETool::EToolDragger);
		return true;
	}

	//
	// Particle system
	//
	else if (Item->GetType() == ETreeViewItemType::ParticleSystem)
	{
		std::shared_ptr<IParticleSystem> objectAsParticleSystem = std::dynamic_pointer_cast<IParticleSystem>(object);
		if (objectAsParticleSystem == nullptr)
			return false;

		CreateDragDataFromParticleSystem(GetBaseManager(), objectAsParticleSystem, Value);
		//GetEditor().GetTools().Enable(ETool::EToolDragger);
		return true;
	}

	return false;
}

bool CEditorResourceBrowser::OnContextMenuTreeItem(const IznTreeViewItem * Item, std::shared_ptr<IPropertiesGroup> PropertiesGroup)
{
	if (Item->GetType() != ETreeViewItemType::VirtualFolder)
		return false;

	PropertiesGroup->SetName("Create");

	auto actionsGroupNEW = MakeShared(CPropertiesGroup, "New", "");

	// Create ParticleSystem
	{
		auto createParticleSystemAction = MakeShared(CAction, "ParticlesSystem", "");
		createParticleSystemAction->SetAction([this, Item]() -> bool {
			auto particleSystem = CreateNewParticle();
			auto particleTreeViewItem = MakeShared(CParticleSystemTreeViewItem, particleSystem);

			auto itemAsVirtualFolder = dynamic_cast<const IznTreeViewItemFolder*>(Item);
			auto itemAsVirtualFolderNonConst = const_cast<IznTreeViewItemFolder*>(itemAsVirtualFolder);
			itemAsVirtualFolderNonConst->AddChild(particleTreeViewItem);

			GetEditorQtUIFrame().getResourcesBrowser()->Refresh();

			return true;
		});
		actionsGroupNEW->AddProperty(createParticleSystemAction);
	}

	// Create Material
	{
		auto createMaterialAction = MakeShared(CAction, "Material", "");
		createMaterialAction->SetAction([this, Item]() -> bool {
			auto material = CreateMaterial();
			auto materialTreeViewItem = MakeShared(CMaterialTreeViewItem, material);

			auto itemAsVirtualFolder = dynamic_cast<const IznTreeViewItemFolder*>(Item);
			auto itemAsVirtualFolderNonConst = const_cast<IznTreeViewItemFolder*>(itemAsVirtualFolder);
			itemAsVirtualFolderNonConst->AddChild(materialTreeViewItem);

			GetEditorQtUIFrame().getResourcesBrowser()->Refresh();

			return true;
		});
		actionsGroupNEW->AddProperty(createMaterialAction);
	}

	PropertiesGroup->AddProperty(actionsGroupNEW);

	return true;
}


std::shared_ptr<IParticleSystem> CEditorResourceBrowser::CreateNewParticle() const
{
	auto particleSystem = MakeShared(CParticleSystem, GetBaseManager());
	particleSystem->SetName("NewParticleSystem");
	particleSystem->SetTexture(GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("star_09.png"));
	return particleSystem;
}

std::shared_ptr<IMaterial> CEditorResourceBrowser::CreateMaterial() const
{
	auto material = MakeShared(MaterialModel, GetBaseManager());
	material->SetName("NewMaterial");
	return material;
}



//
// Private
//
IBaseManager & CEditorResourceBrowser::GetBaseManager() const
{
	return m_Editor.GetBaseManager();
}

IEditor & CEditorResourceBrowser::GetEditor() const
{
	return m_Editor;
}

IEditorUIFrame & CEditorResourceBrowser::GetEditorUIFrame() const
{
	return m_Editor.GetUIFrame();
}

IEditorQtUIFrame & CEditorResourceBrowser::GetEditorQtUIFrame() const
{
	return dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame());
}
