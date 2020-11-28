#include "stdafx.h"

// General
#include "FolderTreeViewItem.h"

CFolderTreeViewItem::CFolderTreeViewItem(const std::string& VirtualFolderName)
	: m_VirtualFolderName(VirtualFolderName)
{
	_ASSERT(false == m_VirtualFolderName.empty());
}

CFolderTreeViewItem::~CFolderTreeViewItem()
{}



//
// IznTreeViewItem
//
ETreeViewItemType CFolderTreeViewItem::GetType() const
{
	return ETreeViewItemType::VirtualFolder;
}

std::string CFolderTreeViewItem::GetText() const
{
	return m_VirtualFolderName;
}

size_t CFolderTreeViewItem::GetChildsCount() const
{
	return m_Childs.size();
}

std::shared_ptr<IznTreeViewItem> CFolderTreeViewItem::GetChild(size_t Index) const
{
	_ASSERT(Index < m_Childs.size());
	return m_Childs.at(Index);
}

std::shared_ptr<IObject> CFolderTreeViewItem::GetObject_() const
{
	return nullptr;
}



//
// IznTreeViewItemFolder
//
void CFolderTreeViewItem::AddChild(std::shared_ptr<IznTreeViewItem> Child)
{
	std::dynamic_pointer_cast<IznTreeViewItemInternal>(Child)->SetParent(this);
	m_Childs.push_back(Child);
}

void CFolderTreeViewItem::ClearChilds()
{
	m_Childs.clear();
}