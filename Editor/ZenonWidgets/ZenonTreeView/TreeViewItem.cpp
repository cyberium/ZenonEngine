#include "stdafx.h"

// General
#include "TreeViewItem.h"

CznTreeViewItem::CznTreeViewItem(const std::shared_ptr<IznTreeViewItemSource>& TObject, CznTreeViewItem * Parent)
	: m_SourceObject(TObject)
	, m_Parent(Parent)
{}

CznTreeViewItem::~CznTreeViewItem()
{
	Log::Warn("TreeViewItem: Destroed TreeView of source with name '%s'", m_SourceObject->GetName().c_str());
}

void CznTreeViewItem::ClearCache()
{
	// TODO: Create great cache system
	//m_CachedChilds.clear();
	
	/*for (auto& it = m_CachedChilds.begin(); it != m_CachedChilds.end(); )
	{
		auto& sourceObject = (*it)->GetSourceObject();

		if (sourceObject.use_count() > 2)
		{
			it++;
			continue;
		}

		it = m_CachedChilds.erase(it);
	}*/
}

std::shared_ptr<CznTreeViewItem> CznTreeViewItem::GetChildByIndex(size_t row)
{
	return GetChildInternal(m_SourceObject->GetChild(row));
}

size_t CznTreeViewItem::GetChildsCount() const
{
	return m_SourceObject->GetChildsCount();
}

CznTreeViewItem* CznTreeViewItem::GetParent()
{
	return m_Parent;
}

QVariant CznTreeViewItem::GetData() const
{
	return QVariant(m_SourceObject->GetName().c_str());
}

size_t CznTreeViewItem::GetMyIndexInParent() const
{
	if (m_Parent)
	{
		for (size_t i = 0; i < m_Parent->GetChildsCount(); i++)
		{
			const auto& ch = m_Parent->GetChildByIndex(i);
			if (ch == shared_from_this())
				return i;
		}
	}

	throw CException("Child not found in parent.");
}

std::shared_ptr<IznTreeViewItemSource> CznTreeViewItem::GetSourceObject() const
{
	return m_SourceObject;
}



//
// Private
//
std::shared_ptr<CznTreeViewItem> CznTreeViewItem::GetChildInternal(const std::shared_ptr<IznTreeViewItemSource>& SourceItem)
{
	if (SourceItem == nullptr)
		return nullptr;

	const auto& sourceName = SourceItem->GetName();

	/*const auto& it = m_CachedChilds.find(sourceName);
	if (it != m_CachedChilds.end())
	{
		auto cachedTreeView = it->second;

		auto cachedSourceObject = cachedTreeView->GetSourceObject();
		_ASSERT(cachedSourceObject != nullptr);

		auto cachedObject = cachedSourceObject->Object();
		if ((cachedObject != nullptr) && (SourceItem->Object() != nullptr))
		{
			if (cachedObject->GetGUID() == SourceItem->Object()->GetGUID())
				return cachedTreeView; // Both sources has objects
		}
		else if ((cachedObject == nullptr) && (SourceItem->Object() == nullptr))
		{
			return cachedTreeView; // Object not contains source
		}
	}*/

	const auto& it = std::find_if(m_CachedChilds.begin(), m_CachedChilds.end(), [sourceName](const std::shared_ptr<CznTreeViewItem>& ChildTreeViewItem)->bool
	{
		auto sourceObject = ChildTreeViewItem->GetSourceObject();
		if (sourceObject == nullptr)
			return false;

		//auto object = sourceObject->Object();
		//if (object == nullptr)
		//	return false;

		//return object == SourceItem->Object();
		return sourceObject->GetName() == sourceName;
	});

	// Retrieve from cahce
	if (it != m_CachedChilds.end())
	{
		return *it;
	}

	// Add new item to cache
	auto newChild = MakeShared(CznTreeViewItem, SourceItem, this);
	//Log::Info("TreeViewItem: Created for source with name '%s'", SourceItem->GetName().c_str());
	//m_CachedChilds.insert(std::make_pair(sourceName, newChild));
	m_CachedChilds.push_back(newChild);
	return newChild;
}
