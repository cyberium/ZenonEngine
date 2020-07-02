#pragma once

#include <QAbstractItemModel>
#include <QModelIndex>

#include "SceneNodeTreeItem.h"

class SceneNodeTreeModel 
	: public QAbstractItemModel 
{
	Q_OBJECT
public:
	SceneNodeTreeModel(QObject *parent = 0);
	virtual ~SceneNodeTreeModel();

	// SceneNodeTreeModel
	void SetModelData(const std::shared_ptr<ISceneNode3D>& SceneNode3D);
	QModelIndex Find(const std::shared_ptr<ISceneNode3D>& Node);

	//
	// QAbstractItemModel
	//
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	QVariant data(const QModelIndex& index, int role) const override;

	Qt::ItemFlags flags(const QModelIndex& index) const override;

	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex& index) const override;

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;


private:
	CSceneNodeTreeItem* getItem(const QModelIndex& index) const;

private:
	CSceneNodeTreeItem* m_RootItem;
};
