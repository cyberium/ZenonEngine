#pragma once

#include "ZenonWidgets/ZenonTreeView/TreeViewItem.h"

class CNodeProtoTreeViewItem
	: public CznTreeViewItem
{
public:
	CNodeProtoTreeViewItem(const std::shared_ptr<ISceneNode>& SceneNode);
	virtual ~CNodeProtoTreeViewItem();

	// IznTreeViewItem
	ETreeViewItemType GetType() const override;
	std::string GetText() const override;
	std::string GetIconName() const override;
	std::shared_ptr<IObject> GetObject_() const override;

private:
	std::shared_ptr<ISceneNode> m_SceneNode;
};