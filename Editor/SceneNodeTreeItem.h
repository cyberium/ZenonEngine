#pragma once

#include <QList>
#include <QVector>
#include <QVariant>

class CSceneNodeTreeItem
{
public:
	CSceneNodeTreeItem();
	CSceneNodeTreeItem(std::shared_ptr<ISceneNode> SceneNode3D, CSceneNodeTreeItem * Parent);
	virtual ~CSceneNodeTreeItem();

	void											addChild(CSceneNodeTreeItem * child);
	CSceneNodeTreeItem*								child(int row); 
	int												childCount() const; 
	CSceneNodeTreeItem*								parentItem();

	QVariant										data() const;
	int												childNumberInParent() const; 

	std::shared_ptr<ISceneNode>                     GetSceneNode() const;

private:
	CSceneNodeTreeItem*								m_Parent;
	std::vector<CSceneNodeTreeItem*>				m_Childs;

	std::shared_ptr<ISceneNode>						m_SceneNode3D;
};
