#include "stdafx.h"

// General
#include "TreeViewItem.h"

CznTreeViewItem::CznTreeViewItem(const IznTreeViewItem * Parent)
	: m_Parent(Parent)
{}

CznTreeViewItem::~CznTreeViewItem()
{
	Log::Warn("TreeViewItem: Destroed TreeView of source with name '%s'", GetText().c_str());
}



//
// IznTreeViewItem
//
ETreeViewItemType CznTreeViewItem::GetType() const
{
	throw CException("Not implemented");
}

std::string CznTreeViewItem::GetText() const
{
	return "Unknown";
}

size_t CznTreeViewItem::GetChildsCount() const
{
	return 0;
}

std::shared_ptr<IznTreeViewItem> CznTreeViewItem::GetChild(size_t Index) const
{
	return nullptr;
}

const IznTreeViewItem* CznTreeViewItem::GetParent() const
{
	return m_Parent;
}

size_t CznTreeViewItem::GetMyIndexInParent() const
{
	if (m_Parent == nullptr)
		throw CException("Parent is nullptr.");

	for (size_t i = 0; i < m_Parent->GetChildsCount(); i++)
	{
		const auto& ch = m_Parent->GetChild(i);
		if (ch == shared_from_this())
			return i;
	}

	throw CException("Child not found in parent.");
}

std::shared_ptr<IObject> CznTreeViewItem::GetObject_() const
{
	throw CException("NotImplemented");
}

void CznTreeViewItem::SetParent(IznTreeViewItem * Parent)
{
	m_Parent = Parent;
}

void CznTreeViewItem::ClearCache()
{
	m_CachedChildMap.clear();
}



//
// protected
//
std::shared_ptr<IznTreeViewItem> CznTreeViewItem::CreateChild(std::shared_ptr<IObject> Object) const
{
	throw CException("NotImplemented");
}

std::shared_ptr<IznTreeViewItem> CznTreeViewItem::GetChildInternal(std::shared_ptr<IObject> Object) const
{
	_ASSERT(Object != nullptr);

	const auto& it = m_CachedChildMap.find(Object->GetGUID());
	if (it != m_CachedChildMap.end())
		return it->second;

	// Add new item to cache
	auto newChild = CreateChild(Object);
	Log::Info("TreeViewItem: Created for source with name '%s'.", GetText().c_str());
	m_CachedChildMap.insert(std::make_pair(Object->GetGUID(), newChild));
	return newChild;
}
