#pragma once

#include "EditorInterfaces.h"
#include "ZenonWidgets/ZenonTreeView/TreeViewIntfs.h"

class CznSceneNode3DTreeViewItemSource
	: public IznTreeViewItemSource
{
public:
	CznSceneNode3DTreeViewItemSource(const std::shared_ptr<ISceneNode>& SceneNode);
	virtual ~CznSceneNode3DTreeViewItemSource();

	// IznTreeViewItemSource
	ETreeViewItemType GetType() const override;
	std::string GetName() const override ;
	size_t GetChildsCount() const override;
	std::shared_ptr<IznTreeViewItemSource> GetChild(size_t Index) const override;
	std::shared_ptr<IObject> Object() const override;

private:
	std::shared_ptr<IznTreeViewItemSource> GetChildInternal(const std::shared_ptr<ISceneNode>& Object) const;

private:
	std::shared_ptr<ISceneNode> m_SceneNode;

	mutable std::map<Guid, std::shared_ptr<CznSceneNode3DTreeViewItemSource>> m_CachedChildMap;
};