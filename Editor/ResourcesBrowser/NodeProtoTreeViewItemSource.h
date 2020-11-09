#pragma once

#include "EditorInterfaces.h"
#include "ZenonWidgets/ZenonTreeView/TreeViewItem.h"

class CzNodeProtoTreeViewItemSource
	: public CznTreeViewItem
{
public:
	CzNodeProtoTreeViewItemSource(const std::shared_ptr<ISceneNode>& SceneNode);
	virtual ~CzNodeProtoTreeViewItemSource();

	// IznTreeViewItemSource
	ETreeViewItemType GetType() const override;
	std::string GetText() const override;
	std::shared_ptr<IObject> GetObject_() const override;

private:
	std::shared_ptr<ISceneNode> m_SceneNode;
};