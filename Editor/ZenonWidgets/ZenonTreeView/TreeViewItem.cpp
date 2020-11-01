#include "stdafx.h"

// General
#include "TreeViewItem.h"

CznTreeViewItem::CznTreeViewItem(const std::shared_ptr<IznTreeViewItemSource>& TObject, CznTreeViewItem * Parent)
	: m_SourceObject(TObject)
	, m_Parent(Parent)
{
	//size_t childsCnt = m_SourceObject->GetChildsCount();
	//for (size_t i = 0; i < childsCnt; i++)
	//	addChild(MakeShared(CznTreeViewItem, m_SourceObject->GetChild(i), this));
}
CznTreeViewItem::~CznTreeViewItem()
{

}

/*
void CznTreeViewItem::SetRoot(const std::shared_ptr<IznTreeViewItemSource>& TObject)
{
	m_SourceObject = TObject;
	size_t childsCnt = m_SourceObject->GetChildsCount();
	for (size_t i = 0; i < childsCnt; i++)
		addChild(MakeShared(CznTreeViewItem, m_SourceObject->GetChild(i), this));
}

void CznTreeViewItem::addChild(std::shared_ptr<CznTreeViewItem> child)
{
	//m_Childs.push_back(child);
}*/

std::shared_ptr<CznTreeViewItem> CznTreeViewItem::child(size_t row)
{
	auto childObject = m_SourceObject->GetChild(row)->Object();

	const auto& it = std::find_if(m_CachedChilds.begin(), m_CachedChilds.end(), [&childObject](const std::shared_ptr<CznTreeViewItem>& ChildTreeViewItem)->bool {
		return ChildTreeViewItem->GetTObject() == childObject;
	});
	if (it != m_CachedChilds.end())
		return *it;

	auto newChild = MakeShared(CznTreeViewItem, m_SourceObject->GetChild(row), this);
	m_CachedChilds.push_back(newChild);
	return newChild;

	//if (row >= m_Childs.size())
	//	return nullptr;
	//return m_Childs.at(row);
}

size_t CznTreeViewItem::childCount() const
{
	return m_SourceObject->GetChildsCount();
	//return m_Childs.size();
}

CznTreeViewItem* CznTreeViewItem::parentItem()
{
	return m_Parent;
}

QVariant CznTreeViewItem::data() const
{
	return QVariant(m_SourceObject->GetName().c_str());
}

int CznTreeViewItem::childNumberInParent() const
{
	/*if (m_Parent)
	{
		const auto& childs = m_Parent->m_Childs;
		const auto& it = std::find(childs.begin(), childs.end(), shared_from_this());
		if (it != m_Parent->m_Childs.end())
			return std::distance(childs.begin(), it);
	}*/

	if (m_Parent)
	{
		for (size_t i = 0; i < m_Parent->childCount(); i++)
		{
			const auto& ch = m_Parent->child(i);
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

std::shared_ptr<IObject> CznTreeViewItem::GetTObject() const
{
	return m_SourceObject->Object();
}