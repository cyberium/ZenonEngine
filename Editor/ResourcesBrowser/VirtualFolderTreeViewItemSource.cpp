#include "stdafx.h"

// General
#include "VirtualFolderTreeViewItemSource.h"

CznVirtualFolderTreeViewItemSource::CznVirtualFolderTreeViewItemSource(const std::string& VirtualFolderName)
	: m_VirtualFolderName(VirtualFolderName)
{
	_ASSERT(false == m_VirtualFolderName.empty());
}

CznVirtualFolderTreeViewItemSource::~CznVirtualFolderTreeViewItemSource()
{}



//
// CznVirtualFolderTreeViewItemSource
//
void CznVirtualFolderTreeViewItemSource::AddChild(std::shared_ptr<IznTreeViewItem> Child)
{
	std::dynamic_pointer_cast<IznTreeViewItemInternal>(Child)->SetParent(this);
	m_Childs.push_back(Child);
}

void CznVirtualFolderTreeViewItemSource::ClearChilds()
{
	m_Childs.clear();
}



//
// IznTreeViewItemSource
//
ETreeViewItemType CznVirtualFolderTreeViewItemSource::GetType() const
{
	return ETreeViewItemType::VirtualFolder;
}

std::string CznVirtualFolderTreeViewItemSource::GetText() const
{
	return m_VirtualFolderName;
}

size_t CznVirtualFolderTreeViewItemSource::GetChildsCount() const
{
	return m_Childs.size();
}

std::shared_ptr<IznTreeViewItem> CznVirtualFolderTreeViewItemSource::GetChild(size_t Index) const
{
	_ASSERT(Index < m_Childs.size());
	return m_Childs.at(Index);
}

std::shared_ptr<IObject> CznVirtualFolderTreeViewItemSource::GetObject_() const
{
	return nullptr;
}
