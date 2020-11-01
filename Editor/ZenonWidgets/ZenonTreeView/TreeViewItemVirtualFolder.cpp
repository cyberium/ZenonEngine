#include "stdafx.h"

// General
#include "TreeViewItemVirtualFolder.h"

CznTreeViewItemVirtualFolder::CznTreeViewItemVirtualFolder(const std::string& VirtualFolderName)
	: m_VirtualFolderName(VirtualFolderName)
{
	_ASSERT(false == m_VirtualFolderName.empty());
}

CznTreeViewItemVirtualFolder::~CznTreeViewItemVirtualFolder()
{}



//
// CznTreeViewItemVirtualFolder
//
void CznTreeViewItemVirtualFolder::AddChild(const std::shared_ptr<IznTreeViewItemSource>& Child)
{
	m_Childs.push_back(Child);
}

void CznTreeViewItemVirtualFolder::ClearChilds()
{
	m_Childs.clear();
}



//
// IznTreeViewItemSource
//
ETreeViewItemType CznTreeViewItemVirtualFolder::GetType() const
{
	return ETreeViewItemType::VirtualFolder;
}

std::string CznTreeViewItemVirtualFolder::GetName() const
{
	return m_VirtualFolderName;
}

size_t CznTreeViewItemVirtualFolder::GetChildsCount() const
{
	return m_Childs.size();
}

std::shared_ptr<IznTreeViewItemSource> CznTreeViewItemVirtualFolder::GetChild(size_t Index) const
{
	_ASSERT(Index < m_Childs.size());
	return m_Childs.at(Index);
}

std::shared_ptr<IObject> CznTreeViewItemVirtualFolder::Object() const
{
	return nullptr;
}
