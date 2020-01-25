#include "stdafx.h"

// General
#include "SceneNodeTreeModel.h"

CSceneNodeTreeModel::CSceneNodeTreeModel(QObject * parent)
	: QAbstractItemModel(parent) 
{
	m_RootItem = new CSceneNodeTreeItem();
}

CSceneNodeTreeModel::~CSceneNodeTreeModel() 
{ 
	delete m_RootItem;
}



//
// CSceneNodeTreeModel
//
void CSceneNodeTreeModel::SetModelData(std::shared_ptr<IScene> Scene3D)
{
	if (m_RootItem)
	{
		delete m_RootItem;
	}

	m_RootItem = new CSceneNodeTreeItem();
	m_RootItem->addChild(new CSceneNodeTreeItem(Scene3D->GetRootNode3D(), m_RootItem));
}



//
// QAbstractItemModel
//
QVariant CSceneNodeTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return m_RootItem->data();

	return QVariant();
}

QVariant CSceneNodeTreeModel::data(const QModelIndex& index, int role) const 
{
	if (!index.isValid()) 
		return QVariant();

	if (role != Qt::DisplayRole) 
		return QVariant();

	CSceneNodeTreeItem *item = getItem(index);
	return item->data();
}

Qt::ItemFlags CSceneNodeTreeModel::flags(const QModelIndex& index) const 
{
	if (!index.isValid()) 
		return 0;

	return QAbstractItemModel::flags(index);
}

QModelIndex CSceneNodeTreeModel::index(int row, int column, const QModelIndex& parent) const 
{
	if (parent.isValid() && parent.column() != 0) 
		return QModelIndex();

	CSceneNodeTreeItem *parentItem = getItem(parent);

	CSceneNodeTreeItem *childItem = parentItem->child(row);
	if (childItem) 
		return createIndex(row, column, childItem);
	else 
		return QModelIndex();
}

QModelIndex CSceneNodeTreeModel::parent(const QModelIndex& index) const 
{
	if (!index.isValid()) 
		return QModelIndex();

	CSceneNodeTreeItem * childItem = getItem(index);
	CSceneNodeTreeItem * parentItem = childItem->parentItem();
	if (parentItem == m_RootItem) 
		return QModelIndex();

	return createIndex(parentItem->childNumberInParent(), 0, parentItem);
}

int CSceneNodeTreeModel::rowCount(const QModelIndex& parent) const 
{
	CSceneNodeTreeItem *parentItem = getItem(parent);
	return parentItem->childCount();
}

int CSceneNodeTreeModel::columnCount(const QModelIndex& parent) const
{
	return 1;
}



//
// Private
//
CSceneNodeTreeItem* CSceneNodeTreeModel::getItem(const QModelIndex& index) const
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
