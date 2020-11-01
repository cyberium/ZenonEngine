#include "stdafx.h"

// General
#include "TreeViewItem.h"

CznTreeViewItem::CznTreeViewItem()
	: m_TObject(nullptr)
	, m_Parent(nullptr)
{}

CznTreeViewItem::CznTreeViewItem(const std::shared_ptr<IznTreeViewItemSource>& TObject, CznTreeViewItem * Parent)
	: m_TObject(TObject)
	, m_Parent(Parent)
{
	size_t childsCnt = m_TObject->GetChildsCount();
	for (size_t i = 0; i < childsCnt; i++)
		addChild(MakeShared(CznTreeViewItem, m_TObject->GetChild(i), this));
}
CznTreeViewItem::~CznTreeViewItem()
{

}

void CznTreeViewItem::SetRoot(const std::shared_ptr<IznTreeViewItemSource>& TObject)
{
	m_TObject = TObject;

	size_t childsCnt = m_TObject->GetChildsCount();
	for (size_t i = 0; i < childsCnt; i++)
		addChild(MakeShared(CznTreeViewItem, m_TObject->GetChild(i), this));
}

void CznTreeViewItem::addChild(std::shared_ptr<CznTreeViewItem> child)
{
	m_Childs.push_back(child);
}

std::shared_ptr<CznTreeViewItem> CznTreeViewItem::child(size_t row)
{
	//if (m_TObject == nullptr)
	//	return nullptr;
	//return MakeShared(CznTreeViewItem, m_TObject->GetChild(row), this);
	if (row >= m_Childs.size())
		return nullptr;
	return m_Childs.at(row);
}

size_t CznTreeViewItem::childCount() const
{
	//if (m_TObject == nullptr)
	//	return 0;
	//return m_TObject->GetChildsCount();
	return m_Childs.size();
}

CznTreeViewItem* CznTreeViewItem::parentItem()
{
	return m_Parent;
}

QVariant CznTreeViewItem::data() const
{
	if (m_TObject == nullptr)
		return QVariant("Root");
	return QVariant(m_TObject->GetName().c_str());
}

int CznTreeViewItem::childNumberInParent() const
{
	if (m_Parent)
	{
		const auto& childs = m_Parent->m_Childs;
		const auto& it = std::find(childs.begin(), childs.end(), shared_from_this());
		if (it != m_Parent->m_Childs.end())
			return std::distance(childs.begin(), it);
	}

	/*if (m_Parent)
	{
		for (size_t i = 0; i < m_Parent->childCount(); i++)
		{
			const auto& ch = m_Parent->child(i);
			if (ch == shared_from_this())
				return i;
		}
	}*/

	throw CException("Child not found in parent.");
}

std::shared_ptr<IObject> CznTreeViewItem::GetTObject() const
{
	return m_TObject->Object();
}