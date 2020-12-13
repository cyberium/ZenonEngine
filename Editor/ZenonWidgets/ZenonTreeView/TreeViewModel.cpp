#include "stdafx.h"

// General
#include "TreeViewModel.h"

// Additional
#include "ResourcesBrowser/TreeViewItems/FolderTreeViewItem.h"

namespace
{
	bool forEach(const QAbstractItemModel* model, QModelIndex parent, const std::shared_ptr<IObject>& Node, QModelIndex * FindedPosition)
	{
		for (int r = 0; r < model->rowCount(parent); ++r)
		{
			QModelIndex index = model->index(r, 0, parent);
			_ASSERT(index.isValid());

			CznTreeViewItem* item = static_cast<CznTreeViewItem*>(index.internalPointer());
			if (item->GetObject_() == Node)
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



CznTreeViewModel::CznTreeViewModel(QObject * parent)
	: QAbstractItemModel(parent)
{
	m_RootItem = MakeShared(CFolderTreeViewItem, "InvisibleRootFolder");
}

CznTreeViewModel::~CznTreeViewModel()
{
}



//
// CznTreeViewModel
//
void CznTreeViewModel::AddToRoot(std::shared_ptr<IznTreeViewItem> Item) const
{
	_ASSERT(m_RootItem != nullptr);
	auto rootVirtualFolderSource = std::dynamic_pointer_cast<IznTreeViewItemFolder>(m_RootItem);
	if (rootVirtualFolderSource == nullptr)
		_ASSERT(false);
	rootVirtualFolderSource->AddChild(Item);
}

void CznTreeViewModel::ClearRoot() const
{
	_ASSERT(m_RootItem != nullptr);
	auto rootVirtualFolderSource = std::dynamic_pointer_cast<IznTreeViewItemFolder>(m_RootItem);
	if (rootVirtualFolderSource == nullptr)
		_ASSERT(false);
	rootVirtualFolderSource->ClearChilds();
	
	ClearRootCache();
}

void CznTreeViewModel::ClearRootCache() const
{
	std::dynamic_pointer_cast<IznTreeViewItemInternal>(m_RootItem)->ClearCache();
}

std::shared_ptr<IznTreeViewItem> CznTreeViewModel::GetRootTreeViewItem() const
{
	return m_RootItem;
}

std::shared_ptr<IObject> CznTreeViewModel::Find(const QModelIndex& ModelIdnex) const
{
	if (!ModelIdnex.isValid())
		return nullptr;

	auto item = static_cast<CznTreeViewItem*>(ModelIdnex.internalPointer());
	return item->GetObject_();
}

QModelIndex CznTreeViewModel::Find(const std::shared_ptr<IObject>& Node) const
{
	QModelIndex findedIndex;
	forEach(this, QModelIndex(), Node, &findedIndex);
	return findedIndex;
}



//
// QAbstractItemModel
//
QVariant CznTreeViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		return QVariant(m_RootItem->GetText().c_str());
	}

	return QVariant();
}

QVariant CznTreeViewModel::data(const QModelIndex& index, int role) const
{
	if (false == index.isValid())
		return QVariant();

	IznTreeViewItem* item = getItem(index);

	if (role == Qt::DisplayRole)
	{
		return QVariant(item->GetText().c_str());
	}
	else if (role == Qt::DecorationRole)
	{
		const std::string& iconName = item->GetIconName();
		if (iconName.empty())
			return QVariant();

		auto icon = GetIcon(item->GetIconName());
		return QVariant(*icon);

		//QPixmap pixmap(QSize(18, 18));
		//pixmap.fill(QColor(88, 255, 88));
		//return QVariant(pixmap);
	}

	return QVariant();
}

Qt::ItemFlags CznTreeViewModel::flags(const QModelIndex& index) const
{
	if (false == index.isValid())
		return Qt::ItemFlag::NoItemFlags;
	return QAbstractItemModel::flags(index) | Qt::ItemFlag::ItemIsDragEnabled;
}

QModelIndex CznTreeViewModel::index(int row, int column, const QModelIndex& parent) const
{
	if (parent.isValid() && parent.column() != 0)
		return QModelIndex();

	IznTreeViewItem* parentItem = getItem(parent);
	auto childItem = parentItem->GetChild(row);
	if (childItem)
	{
		return createIndex(row, column, childItem.get());
	}
	else
	{
		return QModelIndex();
	}
}

QModelIndex CznTreeViewModel::parent(const QModelIndex& index) const
{
	if (false == index.isValid())
		return QModelIndex();

	CznTreeViewItem* childItem = static_cast<CznTreeViewItem*>(index.internalPointer());
	_ASSERT(childItem != nullptr);

	auto parentItem = childItem->GetParent();
	if (parentItem == nullptr)
	{
		_ASSERT(childItem == m_RootItem.get());
		return QModelIndex();
	}

	if (parentItem == m_RootItem.get())
		return QModelIndex();

	return createIndex(parentItem->GetMyIndexInParent(), 0, const_cast<IznTreeViewItem*>(parentItem));
}

int CznTreeViewModel::rowCount(const QModelIndex& parent) const
{
	const IznTreeViewItem* item = getItem(parent);
	return static_cast<int>(item->GetChildsCount());
}

int CznTreeViewModel::columnCount(const QModelIndex& parent) const
{
	return 1;
}

bool CznTreeViewModel::hasChildren(const QModelIndex & parent) const
{
	const IznTreeViewItem* item = getItem(parent);
	return item->GetChildsCount() > 0;
}



//
// Private
//
IznTreeViewItem* CznTreeViewModel::getItem(const QModelIndex& index) const
{
	if (false == index.isValid())
		return m_RootItem.get();

	if (auto item = static_cast<CznTreeViewItem*>(index.internalPointer()))
		return item;

	return m_RootItem.get();
}

QIcon* CznTreeViewModel::GetIcon(const std::string& IconName) const
{
	const auto& iconIt = m_BrowserIconsCache.find(IconName);
	if (iconIt != m_BrowserIconsCache.end())
		return iconIt->second.get();

	std::shared_ptr<QIcon> icon = MakeShared(QIcon);
	icon->addFile(QString::fromUtf8((":/Browser/Resources/Browser/" + IconName + ".png").c_str()), QSize(), QIcon::Normal, QIcon::Off);
 	m_BrowserIconsCache.insert(std::make_pair(IconName, icon));

	return icon.get();
}
