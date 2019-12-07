#pragma once

#include <QAbstractItemModel>
#include <QModelIndex>

#include "SceneNodeTreeItem.h"

class CSceneNodeTreeModel : public QAbstractItemModel 
{
	Q_OBJECT

public:
	CSceneNodeTreeModel(std::shared_ptr<IScene> Scene3D, QObject *parent = 0);
	virtual ~CSceneNodeTreeModel();

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	QVariant data(const QModelIndex& index, int role) const override;

	Qt::ItemFlags flags(const QModelIndex& index) const override;

	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex& index) const override;

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;

private:
	//void setupModelData(const QStringList& lines, CSceneNodeTreeItem *parent);
	CSceneNodeTreeItem* getItem(const QModelIndex& index) const;

private:
	CSceneNodeTreeItem* m_RootItem;
};
