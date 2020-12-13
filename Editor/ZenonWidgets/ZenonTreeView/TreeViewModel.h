#pragma once

#include "TreeViewIntfs.h"

#include <QAbstractItemModel>
#include <QModelIndex>

class CznTreeViewModel
	: public QAbstractItemModel
{
public:
	CznTreeViewModel(QObject *parent = nullptr);
	virtual ~CznTreeViewModel();

	// CznQTTreeViewModel
	void AddToRoot(std::shared_ptr<IznTreeViewItem> Item) const;
	void ClearRoot() const;
	void ClearRootCache() const;
	std::shared_ptr<IznTreeViewItem> GetRootTreeViewItem() const;

	std::shared_ptr<IObject> Find(const QModelIndex& ModelIdnex) const;
	QModelIndex Find(const std::shared_ptr<IObject>& Node) const;

	// QAbstractItemModel
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	QVariant data(const QModelIndex& index, int role) const override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;
	QModelIndex index(int row, int column, const QModelIndex& parent) const override;
	QModelIndex parent(const QModelIndex& index) const override;
	int rowCount(const QModelIndex& parent) const override;
	int columnCount(const QModelIndex& parent) const override;
	bool hasChildren(const QModelIndex& parent) const override;

private:
	IznTreeViewItem* getItem(const QModelIndex& index) const;
	QIcon* GetIcon(const std::string& IconName) const;

private:
	std::shared_ptr<IznTreeViewItem> m_RootItem;
	mutable std::unordered_map<std::string, std::shared_ptr<QIcon>> m_BrowserIconsCache;
};
