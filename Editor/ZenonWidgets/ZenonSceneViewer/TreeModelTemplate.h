#pragma once

#include <QAbstractItemModel>
#include <QModelIndex>

#include "TreeItemTemplate.h"

template <typename T>
class CQtToZenonTreeModel
	: public QAbstractItemModel
{
public:
	CQtToZenonTreeModel(QObject *parent = nullptr)
		: QAbstractItemModel(parent)
	{
		m_RootItem = new CQtToZenonTreeItem<T>(*this);
	}
	virtual ~CQtToZenonTreeModel()
	{
		delete m_RootItem;
	}

	// CQtToZenonTreeModel
	void SetModelData(const std::shared_ptr<T>& SceneNode3D)
	{
		auto oldRootItem = m_RootItem;

		m_RootItem = new CQtToZenonTreeItem<T>(*this);
		m_RootItem->SetRoot(SceneNode3D);

		//if (oldRootItem)
		//	delete oldRootItem;
	}
	std::shared_ptr<T> Find(const QModelIndex& ModelIdnex)
	{
		if (!ModelIdnex.isValid())
			return nullptr;

		auto item = static_cast<CQtToZenonTreeItem<T>*>(ModelIdnex.internalPointer());
		if (item == nullptr)
			return nullptr;

		return item->GetTObject();
	}

	static bool forEach(QAbstractItemModel* model, QModelIndex parent, const std::shared_ptr<T>& Node, QModelIndex * FindedPosition)
	{
		for (int r = 0; r < model->rowCount(parent); ++r)
		{
			QModelIndex index = model->index(r, 0, parent);
			_ASSERT(index.isValid());

			CQtToZenonTreeItem<T>* item = static_cast<CQtToZenonTreeItem<T>*>(index.internalPointer());
			if (item->GetTObject() == Node)
			{
				*FindedPosition = index;
				return true;
			}

			if (model->hasChildren(index))
				if (false == forEach(model, index, Node, FindedPosition))
					return false;
		}

		return false;
	}

	QModelIndex Find(const std::shared_ptr<T>& Node)
	{
		QModelIndex findedIndex;
		forEach(this, QModelIndex(), Node, &findedIndex);
		return findedIndex;
	}
	void Add(Object::Guid Guid, CQtToZenonTreeItem<T>* SceneNodeTreeItem)
	{
		m_Map.insert(std::make_pair(Guid, SceneNodeTreeItem));
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

		if (role == Qt::DisplayRole)
		{
			CQtToZenonTreeItem<T>* item = getItem(index);
			return item->data();
		}
		else if (role == Qt::DecorationRole)
		{
			//if (index.column() == 0)
			//{
			QPixmap pixmap(QSize(24, 24));
			pixmap.fill(QColor(255, 88, 88));
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

		CQtToZenonTreeItem<T>* parentItem = getItem(parent);
		CQtToZenonTreeItem<T>* childItem = parentItem->child(row);
		if (childItem)
			return createIndex(row, column, childItem);
		else
			return QModelIndex();
	}
	QModelIndex parent(const QModelIndex& index) const override
	{
		if (!index.isValid())
			return QModelIndex();

		auto childItem = static_cast<CQtToZenonTreeItem<T>*>(index.internalPointer());
		auto parentItem = childItem->parentItem();

		if (parentItem == m_RootItem)
			return QModelIndex();

		return createIndex(parentItem->childNumberInParent(), 0, parentItem);
	}
	int rowCount(const QModelIndex& parent = QModelIndex()) const override
	{
		if (parent.isValid())
			return static_cast<CQtToZenonTreeItem<T>*>(parent.internalPointer())->childCount();

		return m_RootItem->childCount();
	}
	int columnCount(const QModelIndex& parent = QModelIndex()) const override
	{
		return 1;
	}

private:
	CQtToZenonTreeItem<T>* getItem(const QModelIndex& index) const
	{
		if (!index.isValid())
			return m_RootItem;

		if (auto item = static_cast<CQtToZenonTreeItem<T>*>(index.internalPointer()))
			return item;

		return m_RootItem;
	}

private:
	CQtToZenonTreeItem<T>* m_RootItem;
	std::map<Object::Guid, CQtToZenonTreeItem<T>*> m_Map;
};
