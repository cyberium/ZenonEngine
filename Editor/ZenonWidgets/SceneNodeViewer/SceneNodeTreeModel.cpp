#include "stdafx.h"

// General
#include "SceneNodeTreeModel.h"

SceneNodeTreeModel::SceneNodeTreeModel(QObject * parent)
	: QAbstractItemModel(parent)
{
	m_RootItem = new CSceneNodeTreeItem();
}

SceneNodeTreeModel::~SceneNodeTreeModel()
{
	delete m_RootItem;
}



//
// SceneNodeTreeModel
//
void SceneNodeTreeModel::SetModelData(const std::shared_ptr<ISceneNode3D>& SceneNode3D)
{
	if (m_RootItem)
	{
		delete m_RootItem;
	}

	m_RootItem = new CSceneNodeTreeItem();
	m_RootItem->addChild(new CSceneNodeTreeItem(SceneNode3D, m_RootItem));
}

bool forEach(QAbstractItemModel* model, QModelIndex parent, const std::shared_ptr<ISceneNode3D>& Node, QModelIndex * FindedPosition)
{
	for (int r = 0; r < model->rowCount(parent); ++r)
	{
		QModelIndex index = model->index(r, 0, parent);
		_ASSERT(index.isValid());

		CSceneNodeTreeItem* item = static_cast<CSceneNodeTreeItem*>(index.internalPointer());
		if (item->GetSceneNode() == Node)
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


QModelIndex SceneNodeTreeModel::Find(const std::shared_ptr<ISceneNode3D>& Node)
{
	QModelIndex findedIndex;
	forEach(this, QModelIndex(), Node, &findedIndex);
	return findedIndex;
}



//
// QAbstractItemModel
//
QVariant SceneNodeTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return m_RootItem->data();

	return QVariant();
}

QVariant SceneNodeTreeModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role == Qt::DisplayRole)
	{
		CSceneNodeTreeItem *item = getItem(index);
		return item->data();
	}
	else if (role == Qt::DecorationRole)
	{
		return QColor(255, 88, 88);
	}
		
	return QVariant();
}

Qt::ItemFlags SceneNodeTreeModel::flags(const QModelIndex& index) const 
{
	if (!index.isValid()) 
		return 0;

	return QAbstractItemModel::flags(index);
}

QModelIndex SceneNodeTreeModel::index(int row, int column, const QModelIndex& parent) const 
{
	if (parent.isValid() && parent.column() != 0) 
		return QModelIndex();

	CSceneNodeTreeItem * parentItem = getItem(parent);

	CSceneNodeTreeItem * childItem = parentItem->child(row);
	if (childItem) 
		return createIndex(row, column, childItem);
	else 
		return QModelIndex();
}

QModelIndex SceneNodeTreeModel::parent(const QModelIndex& index) const 
{
	if (!index.isValid())
		return QModelIndex();

	CSceneNodeTreeItem *childItem = static_cast<CSceneNodeTreeItem*>(index.internalPointer());
	CSceneNodeTreeItem *parentItem = childItem->parentItem();

	if (parentItem == m_RootItem)
		return QModelIndex();

	return createIndex(parentItem->childNumberInParent(), 0, parentItem);
}

int SceneNodeTreeModel::rowCount(const QModelIndex& parent) const 
{
	if (parent.isValid())
		return static_cast<CSceneNodeTreeItem*>(parent.internalPointer())->childCount();

	return m_RootItem->childCount();
}

int SceneNodeTreeModel::columnCount(const QModelIndex& parent) const
{
	return 1;
}



//
// Private
//
CSceneNodeTreeItem* SceneNodeTreeModel::getItem(const QModelIndex& index) const
{
	if (!index.isValid())
	{
		return m_RootItem;
	}

	CSceneNodeTreeItem* item = static_cast<CSceneNodeTreeItem*>(index.internalPointer());
	if (item)
	{
		return item;
	}

	return nullptr;
}
