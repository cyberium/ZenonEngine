#include "stdafx.h"

// General
#include "SceneNodeTreeViewItemSource.h"

CznSceneNode3DTreeViewItemSource::CznSceneNode3DTreeViewItemSource(std::shared_ptr<ISceneNode> SceneNode, const IznTreeViewItem* Parent)
	: CznTreeViewItem(Parent)
	, m_SceneNode(SceneNode)
{
}

CznSceneNode3DTreeViewItemSource::~CznSceneNode3DTreeViewItemSource()
{
	Log::Warn("TreeViewSceneNode3DSource: Destroed source of object with name '%s'", m_SceneNode->GetName().c_str());
}


//
// IznTreeViewItemSource
//
ETreeViewItemType CznSceneNode3DTreeViewItemSource::GetType() const
{
	return ETreeViewItemType::CSceneNode;
}

std::string CznSceneNode3DTreeViewItemSource::GetText() const
{
	return m_SceneNode->GetName();
}

size_t CznSceneNode3DTreeViewItemSource::GetChildsCount() const
{
	return m_SceneNode->GetChilds().size();
}

std::shared_ptr<IznTreeViewItem> CznSceneNode3DTreeViewItemSource::GetChild(size_t Index) const
{
	const auto& childs = m_SceneNode->GetChilds();
	_ASSERT(Index < childs.size());
	if (Index >= childs.size())
		return nullptr;

	const auto& child = m_SceneNode->GetChilds().at(Index);
	return GetChildInternal(child);
}

std::shared_ptr<IObject> CznSceneNode3DTreeViewItemSource::GetObject_() const
{
	return m_SceneNode;
}




//
// Protected
//
std::shared_ptr<IznTreeViewItem> CznSceneNode3DTreeViewItemSource::CreateChild(std::shared_ptr<IObject> Object) const
{
	if (auto sceneNode = std::dynamic_pointer_cast<ISceneNode>(Object))
	{
		return MakeShared(CznSceneNode3DTreeViewItemSource, sceneNode, this);
	}
}
