#include "stdafx.h"

// General
#include "UIWindow.h"


CUIWindowNode::CUIWindowNode()
{
}

CUIWindowNode::~CUIWindowNode()
{
	m_Children.clear();
}



//
// Parent & childs functional
//
void CUIWindowNode::AddChild(std::shared_ptr<CUIBaseNode> Node)
{
    _ASSERT(Node != nullptr);

	NodeList::iterator iter = std::find(m_Children.begin(), m_Children.end(), Node);
	if (iter == m_Children.end())
	{
		Node->SetParentInternal(weak_from_this());

		m_Children.push_back(Node);
		std::string nodeName = Node->GetName();
		if (!nodeName.empty())
		{
			m_ChildrenByName.insert(NodeNameMap::value_type(nodeName, Node));
		}
	}
}

void CUIWindowNode::RemoveChild(std::shared_ptr<CUIBaseNode> Node)
{
    _ASSERT(Node != nullptr);

	NodeList::iterator iter = std::find(m_Children.begin(), m_Children.end(), Node);
	if (iter != m_Children.end())
	{
		Node->SetParentInternal(std::weak_ptr<CUIWindowNode>());

		m_Children.erase(iter);

		// Also remove it from the name map.
		NodeNameMap::iterator iter2 = m_ChildrenByName.find(Node->GetName());
		if (iter2 != m_ChildrenByName.end())
		{
			m_ChildrenByName.erase(iter2);
		}
	}
}



//
// CUIBaseNode
//
std::vector<std::shared_ptr<CUIBaseNode>> CUIWindowNode::GetChilds() const
{
    return m_Children;
}
