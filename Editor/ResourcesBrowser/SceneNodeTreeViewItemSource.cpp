#include "stdafx.h"

// General
#include "SceneNodeTreeViewItemSource.h"

CznSceneNode3DTreeViewItemSource::CznSceneNode3DTreeViewItemSource(const std::shared_ptr<ISceneNode>& SceneNode)
	: m_SceneNode(SceneNode)
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

std::string CznSceneNode3DTreeViewItemSource::GetName() const
{
	return m_SceneNode->GetName();
}

size_t CznSceneNode3DTreeViewItemSource::GetChildsCount() const
{
	return m_SceneNode->GetChilds().size();
}

std::shared_ptr<IznTreeViewItemSource> CznSceneNode3DTreeViewItemSource::GetChild(size_t Index) const
{
	const auto& childs = m_SceneNode->GetChilds();
	//_ASSERT(Index < childs.size());
	if (Index >= childs.size())
		return nullptr;

	const auto& child = m_SceneNode->GetChilds().at(Index);
	return GetChildInternal(child);
}
std::shared_ptr<IObject> CznSceneNode3DTreeViewItemSource::Object() const
{
	return m_SceneNode;
}



//
// Private
//
std::shared_ptr<IznTreeViewItemSource> CznSceneNode3DTreeViewItemSource::GetChildInternal(const std::shared_ptr<ISceneNode>& Object) const
{
	if (Object == nullptr)
		return nullptr;

	const auto& it2 = m_CachedChildMap.find(Object->GetGUID());
	if (it2 != m_CachedChildMap.end())
	{
		return it2->second;
	}

	// Add new item to cache
	auto newChild = MakeShared(CznSceneNode3DTreeViewItemSource, Object);
	//Log::Info("TreeViewSceneNode3DSource: Created for object with name '%s'", Object->GetName().c_str());
	m_CachedChildMap.insert(std::make_pair(Object->GetGUID(), newChild));
	return newChild;
}