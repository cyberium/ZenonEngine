#pragma once

#include "ZenonWidgets/ZenonTreeView/TreeViewItem.h"

class CznVirtualFolderTreeViewItemSource
	: public CznTreeViewItem
{
public:
	CznVirtualFolderTreeViewItemSource(const std::string& VirtualFolderName);
	virtual ~CznVirtualFolderTreeViewItemSource();

	// CznVirtualFolderTreeViewItemSource
	void AddChild(std::shared_ptr<IznTreeViewItem> Child);
	void ClearChilds();

	// IznTreeViewItemSource
	ETreeViewItemType GetType() const override;
	std::string GetText() const override;
	size_t GetChildsCount() const override;
	std::shared_ptr<IznTreeViewItem> GetChild(size_t Index) const override;
	std::shared_ptr<IObject> GetObject_() const override;

private:
	std::string m_VirtualFolderName;
	std::vector<std::shared_ptr<IznTreeViewItem>> m_Childs;
};