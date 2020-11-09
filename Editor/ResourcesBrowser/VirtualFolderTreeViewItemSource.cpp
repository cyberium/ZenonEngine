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
void CznVirtualFolderTreeViewItemSource::AddChild(const std::shared_ptr<IznTreeViewItemSource>& Child)
{
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

std::string CznVirtualFolderTreeViewItemSource::GetName() const
{
	return m_VirtualFolderName;
}

size_t CznVirtualFolderTreeViewItemSource::GetChildsCount() const
{
	return m_Childs.size();
}

std::shared_ptr<IznTreeViewItemSource> CznVirtualFolderTreeViewItemSource::GetChild(size_t Index) const
{
	_ASSERT(Index < m_Childs.size());
	return m_Childs.at(Index);
}

std::shared_ptr<IObject> CznVirtualFolderTreeViewItemSource::Object() const
{
	return nullptr;
}
