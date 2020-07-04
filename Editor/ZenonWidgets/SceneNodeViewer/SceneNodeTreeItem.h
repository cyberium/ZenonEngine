#pragma once

#include <QList>
#include <QVector>
#include <QVariant>

class SceneNodeTreeModel;

class CSceneNodeTreeItem
{
public:
	CSceneNodeTreeItem(SceneNodeTreeModel * Model);
	CSceneNodeTreeItem(SceneNodeTreeModel * Model, const std::shared_ptr<ISceneNode3D>& SceneNode3D, CSceneNodeTreeItem * Parent);
	virtual ~CSceneNodeTreeItem();

	void											addChild(CSceneNodeTreeItem * child);
	CSceneNodeTreeItem*								child(int row); 
	int												childCount() const; 
	CSceneNodeTreeItem*								parentItem();

	QVariant										data() const;
	int												childNumberInParent() const; 

	std::shared_ptr<ISceneNode3D>					GetSceneNode() const;

private:
	CSceneNodeTreeItem*								m_Parent;
	std::vector<CSceneNodeTreeItem*>				m_Childs;

	std::shared_ptr<ISceneNode3D>					m_SceneNode3D;
	SceneNodeTreeModel *                            m_Model;
};
