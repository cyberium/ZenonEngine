#include "stdafx.h"

// General
#include "TreeViewModel.h"

namespace
{
	bool forEach(QAbstractItemModel* model, QModelIndex parent, const std::shared_ptr<IObject>& Node, QModelIndex * FindedPosition)
	{
		for (int r = 0; r < model->rowCount(parent); ++r)
		{
			QModelIndex index = model->index(r, 0, parent);
			_ASSERT(index.isValid());

			CznTreeViewItem* item = static_cast<CznTreeViewItem*>(index.internalPointer());
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



CznQTTreeViewModel::CznQTTreeViewModel(QObject * parent)
	: QAbstractItemModel(parent)
{
	m_RootItem = ZN_NEW(CznTreeViewItem);
}

CznQTTreeViewModel::~CznQTTreeViewModel()
{
	delete m_RootItem;
}

// CznQTTreeViewModel
void CznQTTreeViewModel::SetRootItemData(const std::shared_ptr<IznTreeViewItemSource>& Item)
{
	auto oldRoot = m_RootItem;
	m_RootItem = ZN_NEW CznTreeViewItem(Item, nullptr);
	delete oldRoot;
}

void CznQTTreeViewModel::SetChildRootItemsData(const std::vector<std::shared_ptr<IznTreeViewItemSource>>& Items)
{
	for (const auto& item : Items)
		m_RootItem->addChild(MakeShared(CznTreeViewItem, item, m_RootItem));
}

std::shared_ptr<IObject> CznQTTreeViewModel::Find(const QModelIndex& ModelIdnex)
{
	if (!ModelIdnex.isValid())
		return nullptr;

	auto item = static_cast<CznTreeViewItem*>(ModelIdnex.internalPointer());
	if (item == nullptr)
		return nullptr;

	return item->GetTObject();
}

QModelIndex CznQTTreeViewModel::Find(const std::shared_ptr<IObject>& Node)
{
	QModelIndex findedIndex;
	forEach(this, QModelIndex(), Node, &findedIndex);
	return findedIndex;
}



//
// QAbstractItemModel
//
QVariant CznQTTreeViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		return m_RootItem->data();
	}

	return QVariant();
}

QVariant CznQTTreeViewModel::data(const QModelIndex& index, int role) const
{
	if (false == index.isValid())
		return QVariant();

	CznTreeViewItem* item = getItem(index);

	if (role == Qt::DisplayRole)
	{
		return item->data();
	}
	else if (role == Qt::DecorationRole)
	{
		QPixmap pixmap(QSize(18, 18));
		pixmap.fill(QColor(88, 255, 88));
		return QVariant(pixmap);
	}

	return QVariant();
}

Qt::ItemFlags CznQTTreeViewModel::flags(const QModelIndex& index) const
{
	if (false == index.isValid())
		return Qt::ItemFlag::NoItemFlags;
	return QAbstractItemModel::flags(index) | Qt::ItemFlag::ItemIsDragEnabled;
}

QModelIndex CznQTTreeViewModel::index(int row, int column, const QModelIndex& parent) const
{
	if (parent.isValid() && parent.column() != 0)
		return QModelIndex();

	CznTreeViewItem* parentItem = getItem(parent);
	auto childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem.get());
	else
		return QModelIndex();
}

QModelIndex CznQTTreeViewModel::parent(const QModelIndex& index) const
{
	if (false == index.isValid())
		return QModelIndex();

	CznTreeViewItem* childItem = static_cast<CznTreeViewItem*>(index.internalPointer());

	auto parentItem = childItem->parentItem();
	if (parentItem == nullptr)
		return QModelIndex();

	if (parentItem == m_RootItem)
		return QModelIndex();

	return createIndex(parentItem->childNumberInParent(), 0, parentItem);
}

int CznQTTreeViewModel::rowCount(const QModelIndex& parent) const
{
	const CznTreeViewItem* item = getItem(parent);
	return static_cast<int>(item->childCount());
}

int CznQTTreeViewModel::columnCount(const QModelIndex& parent) const
{
	return 1;
}



//
// Private
//
CznTreeViewItem* CznQTTreeViewModel::getItem(const QModelIndex& index) const
{
	if (false == index.isValid())
		return m_RootItem;

	if (auto item = static_cast<CznTreeViewItem*>(index.internalPointer()))
		return item;

	return m_RootItem;
}
