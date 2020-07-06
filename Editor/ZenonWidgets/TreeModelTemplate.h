#pragma once

#include <QAbstractItemModel>
#include <QModelIndex>

#include "TreeItemTemplate.h"

class CQtToZenonTreeModel
	: public QAbstractItemModel
{
public:
	CQtToZenonTreeModel(QObject *parent = nullptr)
		: QAbstractItemModel(parent)
	{
		m_RootItem = new CQtToZenonTreeItem();
	}
	virtual ~CQtToZenonTreeModel()
	{
		delete m_RootItem;
	}

	// CQtToZenonTreeModel
	void SetRootItemData(const std::shared_ptr<IModelCollectionItem>& Item)
	{
		m_RootItem = new CQtToZenonTreeItem();
		m_RootItem->SetRoot(Item);

		//if (oldRootItem)
		//	delete oldRootItem;
	}
	void SetChildRootItemsData(const std::vector<std::shared_ptr<IModelCollectionItem>>& Items)
	{
		for (const auto& item : Items)
			m_RootItem->addChild(new CQtToZenonTreeItem(item, m_RootItem));
	}
	std::shared_ptr<IObject> Find(const QModelIndex& ModelIdnex)
	{
		if (!ModelIdnex.isValid())
			return nullptr;

		auto item = static_cast<CQtToZenonTreeItem*>(ModelIdnex.internalPointer());
		if (item == nullptr)
			return nullptr;

		return item->GetTObject();
	}

	static bool forEach(QAbstractItemModel* model, QModelIndex parent, const std::shared_ptr<IObject>& Node, QModelIndex * FindedPosition)
	{
		for (int r = 0; r < model->rowCount(parent); ++r)
		{
			QModelIndex index = model->index(r, 0, parent);
			_ASSERT(index.isValid());

			CQtToZenonTreeItem* item = static_cast<CQtToZenonTreeItem*>(index.internalPointer());
			if (item->GetTObject() == Node)
			{
				*FindedPosition = index;
				return true;
			}

			if (model->hasChildren(index))
				if (forEach(model, index, Node, FindedPosition))
					return true;
		}

		return false;
	}

	QModelIndex Find(const std::shared_ptr<IObject>& Node)
	{
		QModelIndex findedIndex;
		forEach(this, QModelIndex(), Node, &findedIndex);
		return findedIndex;
	}


	//
	// QAbstractItemModel
	//
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override
	{
		if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		{
			return m_RootItem->data();
		}

		return QVariant();
	}
	QVariant data(const QModelIndex& index, int role) const override
	{
		if (!index.isValid())
			return QVariant();

		CQtToZenonTreeItem* item = getItem(index);

		if (role == Qt::DisplayRole)
		{
			return item->data();
		}
		else if (role == Qt::DecorationRole)
		{
			//if (index.column() == 0)
			//{
			QPixmap pixmap(QSize(18, 18));
			pixmap.fill(QColor(88, 255, 88));
			return QVariant(pixmap); //# √де self.icon это QPixma
		//}
		//return QColor(255, 88, 88);
		}

		return QVariant();
	}
	Qt::ItemFlags flags(const QModelIndex& index) const override
	{
		if (!index.isValid())
			return 0;

		return QAbstractItemModel::flags(index) | Qt::ItemIsDragEnabled;
	}
	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override
	{
		if (parent.isValid() && parent.column() != 0)
			return QModelIndex();

		CQtToZenonTreeItem* parentItem = getItem(parent);
		CQtToZenonTreeItem* childItem = parentItem->child(row);
		if (childItem)
			return createIndex(row, column, childItem);
		else
			return QModelIndex();
	}
	QModelIndex parent(const QModelIndex& index) const override
	{
		if (!index.isValid())
			return QModelIndex();

		auto childItem = static_cast<CQtToZenonTreeItem*>(index.internalPointer());
		auto parentItem = childItem->parentItem();

		if (parentItem == m_RootItem)
			return QModelIndex();

		return createIndex(parentItem->childNumberInParent(), 0, parentItem);
	}
	int rowCount(const QModelIndex& parent = QModelIndex()) const override
	{
		if (parent.isValid())
			return static_cast<CQtToZenonTreeItem*>(parent.internalPointer())->childCount();

		return m_RootItem->childCount();
	}
	int columnCount(const QModelIndex& parent = QModelIndex()) const override
	{
		return 1;
	}

private:
	CQtToZenonTreeItem* getItem(const QModelIndex& index) const
	{
		if (!index.isValid())
			return m_RootItem;

		if (auto item = static_cast<CQtToZenonTreeItem*>(index.internalPointer()))
			return item;

		return m_RootItem;
	}

private:
	CQtToZenonTreeItem* m_RootItem;
	std::map<Guid, CQtToZenonTreeItem*> m_Map;
};
