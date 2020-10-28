#pragma once

#include "SceneBrowserInfs.h"

#include <QVariant>

class CSceneBrowserTreeItem
	: public std::enable_shared_from_this<CSceneBrowserTreeItem>
{
public:
	CSceneBrowserTreeItem();
	CSceneBrowserTreeItem(const std::shared_ptr<IznSceneBrowserNode>& Node, CSceneBrowserTreeItem* Parent);
	virtual ~CSceneBrowserTreeItem();

	void AddChild(std::shared_ptr<IznSceneBrowserNode> ChildNode);
	void RemoveChild(std::shared_ptr<IznSceneBrowserNode> ChildNode);

	size_t GetChildsCount() const;
	std::shared_ptr<CSceneBrowserTreeItem> GetChild(size_t Index) const;
	size_t GetChildIndex(const std::shared_ptr<const CSceneBrowserTreeItem>& Child) const;
	int GetMyIndexInParent() const;
	CSceneBrowserTreeItem* GetParent() const;

	std::string GetText() const;
	std::shared_ptr<IznSceneBrowserNode> GetNode() const;

private:
	CSceneBrowserTreeItem*                              m_Parent;
	std::vector<std::shared_ptr<CSceneBrowserTreeItem>> m_Childs;
	std::shared_ptr<IznSceneBrowserNode>                m_Node;
};
