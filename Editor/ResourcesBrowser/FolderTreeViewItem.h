#pragma once

#include "ZenonWidgets/ZenonTreeView/TreeViewItem.h"

class CFolderTreeViewItem
	: public CznTreeViewItem
	, public IznTreeViewItemFolder
{
public:
	CFolderTreeViewItem(const std::string& VirtualFolderName);
	virtual ~CFolderTreeViewItem();

	// IznTreeViewItem
	ETreeViewItemType GetType() const override;
	std::string GetText() const override;
	size_t GetChildsCount() const override;
	std::shared_ptr<IznTreeViewItem> GetChild(size_t Index) const override;
	std::shared_ptr<IObject> GetObject_() const override;

	// IznTreeViewItemFolder
	void AddChild(std::shared_ptr<IznTreeViewItem> Child);
	void ClearChilds();

private:
	std::string m_VirtualFolderName;
	std::vector<std::shared_ptr<IznTreeViewItem>> m_Childs;
};