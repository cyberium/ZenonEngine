#pragma once

#include "SceneBrowserInfs.h"
#include "SceneBrowserTreeItem.h"

#include <QAbstractItemModel>
#include <QModelIndex>

class CSceneBrowserTreeModel
	: public QAbstractItemModel
{
public:
	CSceneBrowserTreeModel(const std::shared_ptr<IznSceneBrowserNode>& ChildNode, QObject * parent = nullptr);
	virtual ~CSceneBrowserTreeModel();

	// CQtToZenonTreeModel
	std::shared_ptr<IznSceneBrowserNode> Find(const QModelIndex& ModelIdnex);
	QModelIndex                          Find(const std::shared_ptr<IznSceneBrowserNode>& Node);

	// 
	std::shared_ptr<IznSceneBrowserNode> GetRoot() const;
	void                                 AddChildToParent(const std::shared_ptr<IznSceneBrowserNode>& ChildNode, std::shared_ptr<IznSceneBrowserNode> Parent);
	void                                 RemoveChildFromParent(const std::shared_ptr<IznSceneBrowserNode>& ChildNode, std::shared_ptr<IznSceneBrowserNode> Parent);
	size_t                               GetChildsCount(const std::shared_ptr<IznSceneBrowserNode>& Node);
	std::shared_ptr<IznSceneBrowserNode> GetChild(const std::shared_ptr<IznSceneBrowserNode>& Node, size_t Index);
	std::shared_ptr<IznSceneBrowserNode> GetParent(const std::shared_ptr<IznSceneBrowserNode>& Node);

	// QAbstractItemModel
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	QVariant data(const QModelIndex& index, int role) const override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;
	QModelIndex index(int row, int column, const QModelIndex& parent) const override;
	QModelIndex parent(const QModelIndex& index) const override;
	int rowCount(const QModelIndex& parent) const override;
	int columnCount(const QModelIndex& parent) const override;

private:
	CSceneBrowserTreeItem* GetTreeItemFromQTIndex(const QModelIndex& index) const;
	CSceneBrowserTreeItem* FindTreeItem(const std::shared_ptr<IznSceneBrowserNode>& ChildNode);

private:
	CSceneBrowserTreeItem* m_RootNode;
};
