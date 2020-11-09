#pragma once

#include "ZenonWidgets/ZenonTreeView/TreeViewItem.h"

class CznSceneNode3DTreeViewItemSource
	: public CznTreeViewItem
{
public:
	CznSceneNode3DTreeViewItemSource(std::shared_ptr<ISceneNode> SceneNode, const IznTreeViewItem* Parent);
	virtual ~CznSceneNode3DTreeViewItemSource();

	// IznTreeViewItemSource
	ETreeViewItemType GetType() const override;
	std::string GetText() const override ;
	size_t GetChildsCount() const override;
	std::shared_ptr<IznTreeViewItem> GetChild(size_t Index) const override;
	std::shared_ptr<IObject> GetObject_() const override;

protected:
	virtual std::shared_ptr<IznTreeViewItem> CreateChild(std::shared_ptr<IObject> Object) const override;

private:
	std::shared_ptr<ISceneNode> m_SceneNode;
};