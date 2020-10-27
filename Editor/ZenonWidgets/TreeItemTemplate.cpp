#include "stdafx.h"

// General
#include "TreeItemTemplate.h"

const size_t cIncorrectChildIndex = std::numeric_limits<std::size_t>::max();

CQtToZenonTreeItem::CQtToZenonTreeItem()
	: m_TObject(nullptr)
{}

CQtToZenonTreeItem::CQtToZenonTreeItem(const std::shared_ptr<IModelCollectionItem>& TObject, std::weak_ptr<CQtToZenonTreeItem> Parent)
	: m_TObject(TObject)
	, m_Parent(Parent)
{
	for (const auto& ch : m_TObject->GetChilds())
	{
		AddChild(MakeShared(CQtToZenonTreeItem, ch, weak_from_this()));
	}
}

CQtToZenonTreeItem::~CQtToZenonTreeItem()
{
	printf("Test");
}




void CQtToZenonTreeItem::AddChild(std::shared_ptr<CQtToZenonTreeItem> child)
{
	m_Childs.push_back(child);
}

size_t CQtToZenonTreeItem::GetChildsCount() const
{
	return m_Childs.size();
}

std::shared_ptr<CQtToZenonTreeItem> CQtToZenonTreeItem::GetChild(size_t Index) const
{
	if (Index >= m_Childs.size())
		return nullptr;
	return m_Childs.at(Index);
}

size_t CQtToZenonTreeItem::GetChildIndex(const std::shared_ptr<const CQtToZenonTreeItem>& Child) const
{
	const auto& it = std::find(m_Childs.begin(), m_Childs.end(), Child);
	if (it != m_Childs.end())
		return std::distance(m_Childs.begin(), it);
	return cIncorrectChildIndex;
}

std::shared_ptr<CQtToZenonTreeItem> CQtToZenonTreeItem::GetParent() const
{
	if (auto parent = m_Parent.lock())
		return parent;
	return nullptr;
}

int CQtToZenonTreeItem::GetMyIndexInParent() const
{
	auto parent = GetParent();
	if (parent == nullptr)
		return 0;

	size_t index = parent->GetChildIndex(shared_from_this());
	if (index == cIncorrectChildIndex)
		return 0;

	return index;
}



std::string CQtToZenonTreeItem::GetText() const
{
	if (m_TObject == nullptr)
		return "Root";
	return m_TObject->GetName();
}

std::shared_ptr<IObject> CQtToZenonTreeItem::GetTObject() const
{
	return m_TObject->Object();
}