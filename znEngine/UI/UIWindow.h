#pragma once

#include "UIBaseNode.h"

class CUIWindowNode : public CUIBaseNode
{
	typedef CUIBaseNode base;
public:
	CUIWindowNode();
	virtual ~CUIWindowNode();

	// Parent & childs functional
	virtual void AddChild(std::shared_ptr<CUIBaseNode> Node);
	virtual void RemoveChild(std::shared_ptr<CUIBaseNode> Node);
	
	// CUIBaseNode
    virtual std::vector<std::shared_ptr<CUIBaseNode>> GetChilds() const;

private:
	typedef std::vector<std::shared_ptr<CUIBaseNode>> NodeList;
	typedef std::multimap<std::string, std::shared_ptr<CUIBaseNode>> NodeNameMap;

	NodeList				    m_Children;
	NodeNameMap				    m_ChildrenByName;
};