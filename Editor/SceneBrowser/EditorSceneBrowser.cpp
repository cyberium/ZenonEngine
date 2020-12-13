#include "stdafx.h"

// Gerenal
#include "EditorSceneBrowser.h"

// Additional
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
	GetEditorQtUIFrame().getSceneViewer()->SetOnSelectedItemChange(std::bind(&CEditorSceneBrowser::OnSelectTreeItem, this, std::placeholders::_1));
	GetEditorQtUIFrame().getSceneViewer()->SetOnStartDragging(std::bind(&CEditorSceneBrowser::OnStartDraggingTreeItem, this, std::placeholders::_1, std::placeholders::_2));
	GetEditorQtUIFrame().getSceneViewer()->SetOnContexMenu(std::bind(&CEditorSceneBrowser::OnContextMenuTreeItem, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	//UpdateSceneBrowser();
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
bool CEditorSceneBrowser::OnSelectTreeItem(const IznTreeViewItem * Item)
{
	if (Item->GetType() != ETreeViewItemType::CSceneNode)
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

bool CEditorSceneBrowser::OnStartDraggingTreeItem(const IznTreeViewItem * Item, CByteBuffer * Value)
{
	return false;
}

bool CEditorSceneBrowser::OnContextMenuTreeItem(const IznTreeViewItem * Item, std::string * ContextMenuTitle, std::vector<std::shared_ptr<IPropertyAction>> * ResultActions)
{
	if (Item->GetType() != ETreeViewItemType::CSceneNode)
		return false;

	auto object = Item->GetObject_();
	if (object == nullptr)
		return false;

	auto sceneNode3DObject = std::dynamic_pointer_cast<ISceneNode>(object);
	if (sceneNode3DObject == nullptr)
		return false;

	if (false == GetEditorUIFrame().ExtendContextMenu(sceneNode3DObject, ContextMenuTitle, ResultActions))
		return false;

	return true;
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
