#pragma once

#include "ZenonWidgets/ZenonTreeView/TreeViewItem.h"

class CMaterialTreeViewItem
	: public CznTreeViewItem
{
public:
	CMaterialTreeViewItem(const std::shared_ptr<IMaterial>& Material);
	virtual ~CMaterialTreeViewItem();

	// IznTreeViewItem
	ETreeViewItemType GetType() const override;
	std::string GetText() const override;
	std::string GetIconName() const override;
	std::shared_ptr<IObject> GetObject_() const override;

private:
	std::shared_ptr<IMaterial> m_Material;
};