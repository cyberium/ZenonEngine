#include "stdafx.h"

// General
#include "SceneBrowserTreeItem.h"

const size_t cIncorrectChildIndex = std::numeric_limits<std::size_t>::max();

CSceneBrowserTreeItem::CSceneBrowserTreeItem()
	: m_Node(nullptr)
	, m_Parent(nullptr)
{}

CSceneBrowserTreeItem::CSceneBrowserTreeItem(const std::shared_ptr<IznSceneBrowserNode>& TObject, CSceneBrowserTreeItem* Parent)
	: m_Node(TObject)
	, m_Parent(Parent)
{
}

CSceneBrowserTreeItem::~CSceneBrowserTreeItem()
{
	printf("~CSceneBrowserTreeItem");
}




void CSceneBrowserTreeItem::AddChild(std::shared_ptr<IznSceneBrowserNode> ChildNode)
{
	const auto& it = std::find_if(m_Childs.begin(), m_Childs.end(), [&ChildNode](const std::shared_ptr<CSceneBrowserTreeItem>& Value) -> bool {
		return Value->GetNode() == ChildNode;
	});
	if (it != m_Childs.end())
	{
		Log::Error("CSceneBrowserTreeItem: Child '%s' already exists in parent '%s'.", ChildNode->GetText().c_str(), GetText().c_str());
		return;
	}
	m_Childs.push_back(MakeShared(CSceneBrowserTreeItem, ChildNode, this));
}

void CSceneBrowserTreeItem::RemoveChild(std::shared_ptr<IznSceneBrowserNode> ChildNode)
{
	const auto& it = std::find_if(m_Childs.begin(), m_Childs.end(), [&ChildNode] (const std::shared_ptr<CSceneBrowserTreeItem>& Value) -> bool {
		return Value->GetNode() == ChildNode;
	});
	if (it == m_Childs.end())
	{
		Log::Error("CSceneBrowserTreeItem: Unable to child '%s' in parent '%s'.", ChildNode->GetText().c_str(), GetText().c_str());
		return;
	}
	m_Childs.erase(it);
}

size_t CSceneBrowserTreeItem::GetChildsCount() const
{
	return m_Childs.size();
}

std::shared_ptr<CSceneBrowserTreeItem> CSceneBrowserTreeItem::GetChild(size_t Index) const
{
	if (Index >= m_Childs.size())
		return nullptr;
	return m_Childs.at(Index);
}

size_t CSceneBrowserTreeItem::GetChildIndex(const std::shared_ptr<const CSceneBrowserTreeItem>& Child) const
{
	const auto& it = std::find(m_Childs.begin(), m_Childs.end(), Child);
	if (it != m_Childs.end())
		return std::distance(m_Childs.begin(), it);
	return cIncorrectChildIndex;
}

CSceneBrowserTreeItem* CSceneBrowserTreeItem::GetParent() const
{
	return m_Parent;
}

int CSceneBrowserTreeItem::GetMyIndexInParent() const
{
	auto parent = GetParent();
	if (parent == nullptr)
		return 0;
	size_t index = parent->GetChildIndex(shared_from_this());
	if (index == cIncorrectChildIndex)
		return 0;
	return index;
}



std::string CSceneBrowserTreeItem::GetText() const
{
	if (m_Node == nullptr)
		return "<Root>";
	return m_Node->GetText();
}

std::shared_ptr<IznSceneBrowserNode> CSceneBrowserTreeItem::GetNode() const
{
	return m_Node;
}