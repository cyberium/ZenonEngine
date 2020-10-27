#pragma once

#include <QAbstractItemModel>
#include <QModelIndex>

#include "TreeItemTemplate.h"

class CQtToZenonTreeModel
	: public QAbstractItemModel
{
public:
	CQtToZenonTreeModel(QObject *parent = nullptr);
	virtual ~CQtToZenonTreeModel();

	// CQtToZenonTreeModel
	void SetRootItemData(const std::shared_ptr<IModelCollectionItem>& Item);
	void SetChildRootItemsData(const std::vector<std::shared_ptr<IModelCollectionItem>>& Items);
	std::shared_ptr<IObject> Find(const QModelIndex& ModelIdnex);
	QModelIndex Find(const std::shared_ptr<IObject>& Node);

	// QAbstractItemModel
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	QVariant data(const QModelIndex& index, int role) const override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;
	QModelIndex index(int row, int column, const QModelIndex& parent) const override;
	QModelIndex parent(const QModelIndex& index) const override;
	int rowCount(const QModelIndex& parent) const override;
	int columnCount(const QModelIndex& parent) const override;

private:
	CQtToZenonTreeItem* getItem(const QModelIndex& index) const;

private:
	std::shared_ptr<CQtToZenonTreeItem> m_RootItem;
};
