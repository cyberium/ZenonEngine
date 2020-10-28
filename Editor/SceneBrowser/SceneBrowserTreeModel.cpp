#include "stdafx.h"

// General
#include "SceneBrowserTreeModel.h"


namespace
{
	bool forEach(QAbstractItemModel* model, QModelIndex parent, const std::shared_ptr<IznSceneBrowserNode>& Node, QModelIndex * FindedPosition)
	{
		for (int r = 0; r < model->rowCount(parent); ++r)
		{
			QModelIndex index = model->index(r, 0, parent);
			_ASSERT(index.isValid());

			CSceneBrowserTreeItem* item = static_cast<CSceneBrowserTreeItem*>(index.internalPointer());
			if (item->GetNode() == Node)
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



CSceneBrowserTreeModel::CSceneBrowserTreeModel(const std::shared_ptr<IznSceneBrowserNode>& ChildNode, QObject *parent)
	: QAbstractItemModel(parent)
{
	m_RootNode = ZN_NEW CSceneBrowserTreeItem(ChildNode, nullptr);
}

CSceneBrowserTreeModel::~CSceneBrowserTreeModel()
{
}



//
// CSceneBrowserTreeModel
//
std::shared_ptr<IznSceneBrowserNode> CSceneBrowserTreeModel::Find(const QModelIndex& QTIndex)
{
	return GetTreeItemFromQTIndex(QTIndex)->GetNode();
}

QModelIndex CSceneBrowserTreeModel::Find(const std::shared_ptr<IznSceneBrowserNode>& Node)
{
	QModelIndex findedIndex;
	forEach(this, QModelIndex(), Node, &findedIndex);
	return findedIndex;
}



std::shared_ptr<IznSceneBrowserNode> CSceneBrowserTreeModel::GetRoot() const
{
	return std::shared_ptr<IznSceneBrowserNode>();
}

void CSceneBrowserTreeModel::AddChildToParent(const std::shared_ptr<IznSceneBrowserNode>& ChildNode, std::shared_ptr<IznSceneBrowserNode> ParentNode)
{
	auto parentTreeItem = FindTreeItem(ParentNode);
	if (parentTreeItem == nullptr)
	{
		Log::Error("SceneBrowserTreeModel: Unable to find child '%s'.", ChildNode->GetText().c_str());
		return;
	}
	parentTreeItem->AddChild(ChildNode);
}

void CSceneBrowserTreeModel::RemoveChildFromParent(const std::shared_ptr<IznSceneBrowserNode>& ChildNode, std::shared_ptr<IznSceneBrowserNode> ParentNode)
{
	auto parentTreeItem = FindTreeItem(ParentNode);
	if (parentTreeItem == nullptr)
	{
		Log::Error("SceneBrowserTreeModel: Unable to find child '%s'.", ChildNode->GetText().c_str());
		return;
	}
	parentTreeItem->RemoveChild(ChildNode);
}

size_t CSceneBrowserTreeModel::GetChildsCount(const std::shared_ptr<IznSceneBrowserNode>& Node)
{
	auto treeItem = FindTreeItem(Node);
	if (treeItem == nullptr)
	{
		Log::Error("SceneBrowserTreeModel: Unable to find node '%s'.", Node->GetText().c_str());
		return 0;
	}
	return treeItem->GetChildsCount();
}

std::shared_ptr<IznSceneBrowserNode> CSceneBrowserTreeModel::GetChild(const std::shared_ptr<IznSceneBrowserNode>& Node, size_t Index)
{
	auto treeItem = FindTreeItem(Node);
	if (treeItem == nullptr)
	{
		Log::Error("SceneBrowserTreeModel: Unable to find node '%s'.", Node->GetText().c_str());
		return GetRoot();
	}
	return treeItem->GetChild(Index)->GetNode();
}

std::shared_ptr<IznSceneBrowserNode> CSceneBrowserTreeModel::GetParent(const std::shared_ptr<IznSceneBrowserNode>& Node)
{
	auto treeItem = FindTreeItem(Node);
	if (treeItem == nullptr)
	{
		Log::Error("SceneBrowserTreeModel: Unable to find node '%s'.", Node->GetText().c_str());
		return GetRoot();
	}
	return treeItem->GetParent()->GetNode();
}



//
// QAbstractItemModel
//
QVariant CSceneBrowserTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return QVariant(m_RootNode->GetText().c_str());

	return QVariant();
}

QVariant CSceneBrowserTreeModel::data(const QModelIndex& QTIndex, int role) const
{
	if (false == QTIndex.isValid())
		return QVariant();

	if (role == Qt::DisplayRole)
	{
		return QVariant(GetTreeItemFromQTIndex(QTIndex)->GetText().c_str());
	}
	else if (role == Qt::DecorationRole)
	{
		QPixmap pixmap(QSize(18, 18));
		pixmap.fill(QColor(88, 255, 88));
		return QVariant(pixmap); 
	}

	return QVariant();
}

Qt::ItemFlags CSceneBrowserTreeModel::flags(const QModelIndex& QTIndex) const
{
	if (false == QTIndex.isValid())
		return Qt::NoItemFlags;
	return QAbstractItemModel::flags(QTIndex) | Qt::ItemIsDragEnabled;
}

QModelIndex CSceneBrowserTreeModel::index(int row, int column, const QModelIndex& QTIndex) const
{
	auto childItem = GetTreeItemFromQTIndex(QTIndex)->GetChild(row);
	if (childItem == nullptr)
		return QModelIndex();

	return createIndex(row, column, childItem.get());
}

QModelIndex CSceneBrowserTreeModel::parent(const QModelIndex& QTIndex) const
{
	auto parentItem = GetTreeItemFromQTIndex(QTIndex)->GetParent();
	//if (parentItem == nullptr)
	//	return QModelIndex();

	if (parentItem == m_RootNode)
		return QModelIndex();

	return createIndex(parentItem->GetMyIndexInParent(), 0, parentItem);
}

int CSceneBrowserTreeModel::rowCount(const QModelIndex& QTIndex) const
{
	return GetTreeItemFromQTIndex(QTIndex)->GetChildsCount();
}

int CSceneBrowserTreeModel::columnCount(const QModelIndex& QTIndex) const
{
	return 1;
}



//
// Private
//
CSceneBrowserTreeItem* CSceneBrowserTreeModel::GetTreeItemFromQTIndex(const QModelIndex& QTIndex) const
{
	if (false == QTIndex.isValid())
		return m_RootNode;

	CSceneBrowserTreeItem* item = static_cast<CSceneBrowserTreeItem*>(QTIndex.internalPointer());
	if (item == nullptr)
		return m_RootNode;

	return item;
}

namespace
{
	CSceneBrowserTreeItem* FindChildRec(CSceneBrowserTreeItem* From, const std::shared_ptr<IznSceneBrowserNode>& ElementToFind)
	{
		_ASSERT(From != nullptr);

		if (From->GetNode()->GetId() == ElementToFind->GetId())
			return From;

		for (size_t i = 0; i < From->GetChildsCount(); i++)
			return FindChildRec(From->GetChild(i).get(), ElementToFind);

		return nullptr;
	}	
}

CSceneBrowserTreeItem* CSceneBrowserTreeModel::FindTreeItem(const std::shared_ptr<IznSceneBrowserNode>& ChildNode)
{
	return FindChildRec(m_RootNode, ChildNode);
}
