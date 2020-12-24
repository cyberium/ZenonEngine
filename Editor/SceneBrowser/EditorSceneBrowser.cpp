#include "stdafx.h"

// Gerenal
#include "EditorSceneBrowser.h"

// Additional
#include "EditorUI/DragUtils.h"

#include "SceneNodeTreeViewItem.h"

CEditorSceneBrowser::CEditorSceneBrowser(IEditor& Editor)
	: m_Editor(Editor)
	, IsAttachedTest(false)
{
}

CEditorSceneBrowser::~CEditorSceneBrowser()
{
}



//
// CEditorSceneBrowser
//
void CEditorSceneBrowser::Initialize()
{
	GetEditorQtUIFrame().getSceneViewer()->SetOnContexMenu(std::bind(&CEditorSceneBrowser::OnContextMenuTreeItem, this, std::placeholders::_1, std::placeholders::_2));

	GetEditorQtUIFrame().getSceneViewer()->SetOnSelectedItemChange(std::bind(&CEditorSceneBrowser::OnSelectTreeItem, this, std::placeholders::_1));
	GetEditorQtUIFrame().getSceneViewer()->SetOnStartDragging(std::bind(&CEditorSceneBrowser::OnStartDraggingTreeItem, this, std::placeholders::_1, std::placeholders::_2));
	
	GetEditorQtUIFrame().getSceneViewer()->SetOnDragMove(std::bind(&CEditorSceneBrowser::OnDragMoveTreeItem, this, std::placeholders::_1, std::placeholders::_2));
	GetEditorQtUIFrame().getSceneViewer()->SetOnDragDrop(std::bind(&CEditorSceneBrowser::OnDragDropTreeItem, this, std::placeholders::_1, std::placeholders::_2));

	GetEditorQtUIFrame().getSceneViewer()->setSelectionMode(QAbstractItemView::ExtendedSelection);

}

void CEditorSceneBrowser::Update()
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
// Events
//

bool CEditorSceneBrowser::OnContextMenuTreeItem(const IznTreeViewItem * Item, std::shared_ptr<IPropertiesGroup> PropertiesGroup)
{
	if (Item->GetType() != ETreeViewItemType::SceneNode)
		return false;

	auto object = Item->GetObject_();
	if (object == nullptr)
		return false;

	auto sceneNode3DObject = std::dynamic_pointer_cast<ISceneNode>(object);
	if (sceneNode3DObject == nullptr)
		return false;

	if (false == GetEditorUIFrame().ExtendContextMenu(sceneNode3DObject, PropertiesGroup))
		return false;

	return true;
}


bool CEditorSceneBrowser::OnSelectTreeItem(const IznTreeViewItem * Item)
{
	if (Item->GetType() != ETreeViewItemType::SceneNode)
		return false;

	auto object = Item->GetObject_();
	if (object == nullptr)
		return false;

	auto sceneNode3DObject = std::dynamic_pointer_cast<ISceneNode>(object);
	if (sceneNode3DObject == nullptr)
		return false;

	m_Editor.GetTools().GetToolT<IEditorToolSelector>(ETool::EToolSelector).SelectNode(sceneNode3DObject);
	return true;
}

bool CEditorSceneBrowser::OnStartDraggingTreeItem(const IznTreeViewItem* Item, CByteBuffer* ByteBuffer)
{
	if (ByteBuffer == nullptr || ByteBuffer->getSize() > 0 || ByteBuffer->getPos() > 0)
		throw CException("Unable to 'OnStartDraggingTreeItem', because ByteBuffer is not empty.");

	auto object = Item->GetObject_();
	if (object == nullptr)
		return false;

	//
	// Existing scene node
	//
	if (Item->GetType() == ETreeViewItemType::SceneNode)
	{
		auto objectAsModelAsSceneNode = std::dynamic_pointer_cast<ISceneNode>(object);
		if (objectAsModelAsSceneNode == nullptr)
			return false;

		CreateDragDataFromSceneNode(objectAsModelAsSceneNode, ByteBuffer);
		//m_Editor.GetTools().Enable(ETool::EToolDragger);
		return true;
	}
	else
		throw CException("Unexpected behaviour.");
}


bool CEditorSceneBrowser::OnDragMoveTreeItem(const IznTreeViewItem * TreeViewItem, const CByteBuffer& ByteBuffer)
{
	auto destinationObject = TreeViewItem->GetObject_();
	if (destinationObject == nullptr)
		return false;

	auto destinationSceneNode = std::dynamic_pointer_cast<ISceneNode>(destinationObject);
	if (destinationSceneNode == nullptr)
		return false;

	ETreeViewItemType dragDataSourceType = GetDragDataSourceType(ByteBuffer);
	if (dragDataSourceType == ETreeViewItemType::SceneNode)
	{
		Guid sourceSceneNodeGuid = GetSceneNodeFromDragData(GetBaseManager(), ByteBuffer);
		
		// Same nodes not available
		if (sourceSceneNodeGuid == destinationSceneNode->GetGUID())
		{
			Log::Warn("OnDragMove: Destination node is same source node.");
			return false;
		}
		
		auto sourceSceneNode = m_Editor.Get3DFrame().GetEditedScene()->GetFinder().FindNodeByGuid(sourceSceneNodeGuid);
		if (sourceSceneNode == nullptr)
			throw CException("OnDragMove: SceneNode with Guid '%s' not found.", sourceSceneNodeGuid.CStr());

		if (IsChildOf(sourceSceneNode, destinationSceneNode))
		{
			Log::Warn("OnDragMove: Destination node '%s' is child of source node '%s'.", TreeViewItem->GetText().c_str(), sourceSceneNode->GetName().c_str());
			return false;
		}

		//Log::Green("OnDragMove: Destination node '%s'. Source node '%s'.", TreeViewItem->GetText().c_str(), sourceSceneNode->GetName().c_str());
		return true;
	}
	else
		throw CException("Unexpected behaviour. Inknown drag data source '%d'.", dragDataSourceType);

	return false;
}

bool CEditorSceneBrowser::OnDragDropTreeItem(const IznTreeViewItem * TreeViewItem, const CByteBuffer& ByteBuffer)
{
	auto destinationObject = TreeViewItem->GetObject_();
	if (destinationObject == nullptr)
		return false;

	auto destinationSceneNode = std::dynamic_pointer_cast<ISceneNode>(destinationObject);
	if (destinationSceneNode == nullptr)
		return false;

	ETreeViewItemType dragDataSourceType = GetDragDataSourceType(ByteBuffer);
	if (dragDataSourceType == ETreeViewItemType::SceneNode)
	{
		Guid sourceSceneNodeGuid = GetSceneNodeFromDragData(GetBaseManager(), ByteBuffer);
			   
		// Same nodes not available
		if (sourceSceneNodeGuid == destinationSceneNode->GetGUID())
			throw CException("OnDragDrop: Destination node is same source node.");

		auto sourceSceneNode = m_Editor.Get3DFrame().GetEditedScene()->GetFinder().FindNodeByGuid(sourceSceneNodeGuid);
		if (sourceSceneNode == nullptr)
			throw CException("OnDragDrop: SceneNode with Guid '%s' not found.", sourceSceneNodeGuid.CStr());

		if (IsChildOf(sourceSceneNode, destinationSceneNode))
			throw CException("OnDragDrop: Destination node '%s'. Source node '%s'.", TreeViewItem->GetText().c_str(), sourceSceneNode->GetName().c_str());

		auto worldPosition = sourceSceneNode->GetWorldTransfom();
		destinationSceneNode->AddChild(sourceSceneNode);
		sourceSceneNode->SetWorldTransform(worldPosition);

		Log::Green("OnDragDrop: Destination node '%s'. Source node '%s'.", TreeViewItem->GetText().c_str(), sourceSceneNode->GetName().c_str());
		return true;
	}
	else
		throw CException("Unexpected behaviour. Inknown drag data source '%d'.", dragDataSourceType);

	return false;
}



//
// Private
//
IBaseManager & CEditorSceneBrowser::GetBaseManager() const
{
	return m_Editor.GetBaseManager();
}

IEditorUIFrame & CEditorSceneBrowser::GetEditorUIFrame() const
{
	return m_Editor.GetUIFrame();
}

IEditorQtUIFrame& CEditorSceneBrowser::GetEditorQtUIFrame() const
{
	return dynamic_cast<IEditorQtUIFrame&>(m_Editor.GetUIFrame());
}
