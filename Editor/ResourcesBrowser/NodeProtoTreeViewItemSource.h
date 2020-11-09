#pragma once

#include "EditorInterfaces.h"
#include "ZenonWidgets/ZenonTreeView/TreeViewIntfs.h"

class CzNodeProtoTreeViewItemSource
	: public IznTreeViewItemSource
{
public:
	CzNodeProtoTreeViewItemSource(const std::shared_ptr<ISceneNode>& SceneNode);
	virtual ~CzNodeProtoTreeViewItemSource();

	// IznTreeViewItemSource
	ETreeViewItemType GetType() const override;
	std::string GetName() const override;
	size_t GetChildsCount() const override;
	std::shared_ptr<IznTreeViewItemSource> GetChild(size_t Index) const override;
	bool IsEqual(const std::shared_ptr<IznTreeViewItemSource>& Other) const;
	std::shared_ptr<IObject> Object() const override;

private:
	std::shared_ptr<ISceneNode> m_SceneNode;
};