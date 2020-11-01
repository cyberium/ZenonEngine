#pragma once

#include "TreeViewItem.h"

#include <QAbstractItemModel>
#include <QModelIndex>

class CznQTTreeViewModel
	: public QAbstractItemModel
{
public:
	CznQTTreeViewModel(QObject *parent = nullptr);
	virtual ~CznQTTreeViewModel();

	// CznQTTreeViewModel
	//void ReplaceRoot(const std::shared_ptr<IznTreeViewItemSource>& Item);
	void AddToRoot(const std::shared_ptr<IznTreeViewItemSource>& Item) const;
	void ClearRoot() const;
	void ClearRootCache() const;

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
	CznTreeViewItem* getItem(const QModelIndex& index) const;

private:
	std::shared_ptr<CznTreeViewItem> m_RootItem;
};
