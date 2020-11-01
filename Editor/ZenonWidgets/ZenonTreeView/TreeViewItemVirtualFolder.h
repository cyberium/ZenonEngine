#pragma once

#include "TreeViewIntfs.h"

class CznTreeViewItemVirtualFolder
	: public IznTreeViewItemSource
{
public:
	CznTreeViewItemVirtualFolder(const std::string& VirtualFolderName);
	virtual ~CznTreeViewItemVirtualFolder();

	// CznVirtualFolderTreeViewItemSource
	void AddChild(const std::shared_ptr<IznTreeViewItemSource>& Child);
	void ClearChilds();

	// IznTreeViewItemSource
	ETreeViewItemType GetType() const override;
	std::string GetName() const override;
	size_t GetChildsCount() const override;
	std::shared_ptr<IznTreeViewItemSource> GetChild(size_t Index) const override;
	std::shared_ptr<IObject> Object() const override;

private:
	std::string m_VirtualFolderName;
	std::vector<std::shared_ptr<IznTreeViewItemSource>> m_Childs;
};