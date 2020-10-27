#include "stdafx.h"

// General
#include "TreeModelTemplate.h"


namespace
{

	bool forEach(QAbstractItemModel* model, QModelIndex parent, const std::shared_ptr<IObject>& Node, QModelIndex * FindedPosition)
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
}



CQtToZenonTreeModel::CQtToZenonTreeModel(QObject *parent)
	: QAbstractItemModel(parent)
{
	m_RootItem = MakeShared(CQtToZenonTreeItem);
}

CQtToZenonTreeModel::~CQtToZenonTreeModel()
{
}



//
// CQtToZenonTreeModel
//
void CQtToZenonTreeModel::SetRootItemData(const std::shared_ptr<IModelCollectionItem>& Item)
{
	m_RootItem = MakeShared(CQtToZenonTreeItem, Item, std::weak_ptr<CQtToZenonTreeItem>());
}

void CQtToZenonTreeModel::SetChildRootItemsData(const std::vector<std::shared_ptr<IModelCollectionItem>>& Items)
{
	for (const auto& item : Items)
		m_RootItem->AddChild(MakeShared(CQtToZenonTreeItem, item, m_RootItem));
}

std::shared_ptr<IObject> CQtToZenonTreeModel::Find(const QModelIndex& QTIndex)
{
	return getItem(QTIndex)->GetTObject();
}

QModelIndex CQtToZenonTreeModel::Find(const std::shared_ptr<IObject>& Node)
{
	QModelIndex findedIndex;
	forEach(this, QModelIndex(), Node, &findedIndex);
	return findedIndex;
}



//
// QAbstractItemModel
//
QVariant CQtToZenonTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		return QVariant(m_RootItem->GetText().c_str());
	}

	return QVariant();
}

QVariant CQtToZenonTreeModel::data(const QModelIndex& QTIndex, int role) const
{
	if (false == QTIndex.isValid())
		return QVariant();

	if (role == Qt::DisplayRole)
	{
		return QVariant(getItem(QTIndex)->GetText().c_str());
	}
	else if (role == Qt::DecorationRole)
	{
		QPixmap pixmap(QSize(18, 18));
		pixmap.fill(QColor(88, 255, 88));
		return QVariant(pixmap); 
	}

	return QVariant();
}

Qt::ItemFlags CQtToZenonTreeModel::flags(const QModelIndex& QTIndex) const
{
	if (false == QTIndex.isValid())
		return Qt::NoItemFlags;
	return QAbstractItemModel::flags(QTIndex) | Qt::ItemIsDragEnabled;
}

QModelIndex CQtToZenonTreeModel::index(int row, int column, const QModelIndex& QTIndex) const
{
	auto childItem = getItem(QTIndex)->GetChild(row);
	if (childItem == nullptr)
	{
		_ASSERT(false);
		return QModelIndex();
	}

	return createIndex(row, column, childItem.get());
}

QModelIndex CQtToZenonTreeModel::parent(const QModelIndex& QTIndex) const
{
	auto parentItem = getItem(QTIndex)->GetParent();
	if (parentItem == nullptr)
		return QModelIndex();

	if (parentItem == m_RootItem)
		return QModelIndex();

	return createIndex(parentItem->GetMyIndexInParent(), 0, parentItem.get());
}

int CQtToZenonTreeModel::rowCount(const QModelIndex& QTIndex) const
{
	return getItem(QTIndex)->GetChildsCount();
}

int CQtToZenonTreeModel::columnCount(const QModelIndex& QTIndex) const
{
	return 1;
}



//
// Private
//
CQtToZenonTreeItem* CQtToZenonTreeModel::getItem(const QModelIndex& QTIndex) const
{
	if (false == QTIndex.isValid())
		return m_RootItem.get();

	CQtToZenonTreeItem* item = static_cast<CQtToZenonTreeItem*>(QTIndex.internalPointer());
	if (item == nullptr)
		return m_RootItem.get();

	return item;
}