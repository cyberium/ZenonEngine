#include "stdafx.h"

// General
#include "SceneNodeTreeViewItem.h"

CSceneNodeTreeViewItem::CSceneNodeTreeViewItem(std::shared_ptr<ISceneNode> SceneNode, const IznTreeViewItem* Parent)
	: CznTreeViewItem(Parent)
	, m_SceneNode(SceneNode)
{
}

CSceneNodeTreeViewItem::~CSceneNodeTreeViewItem()
{
	Log::Warn("TreeViewSceneNode3DSource: Destroed source of object with name '%s'", m_SceneNode->GetName().c_str());
}


//
// IznTreeViewItem
//
ETreeViewItemType CSceneNodeTreeViewItem::GetType() const
{
	return ETreeViewItemType::CSceneNode;
}

std::string CSceneNodeTreeViewItem::GetText() const
{
	return m_SceneNode->GetName() /*+ "#" + std::to_string(m_SceneNode->GetGUID().GetCounter())*/;
}

size_t CSceneNodeTreeViewItem::GetChildsCount() const
{
	return m_SceneNode->GetChilds().size();
}

std::shared_ptr<IznTreeViewItem> CSceneNodeTreeViewItem::GetChild(size_t Index) const
{
	const auto& childs = m_SceneNode->GetChilds();
	if (Index >= childs.size())
		return nullptr;

	const auto& child = m_SceneNode->GetChilds().at(Index);
	return GetChildInternal(child);
}

std::shared_ptr<IObject> CSceneNodeTreeViewItem::GetObject_() const
{
	return m_SceneNode;
}




//
// Protected
//
std::shared_ptr<IznTreeViewItem> CSceneNodeTreeViewItem::CreateChild(std::shared_ptr<IObject> Object) const
{
	if (auto sceneNode = std::dynamic_pointer_cast<ISceneNode>(Object))
	{
		return MakeShared(CSceneNodeTreeViewItem, sceneNode, this);
	}
}
